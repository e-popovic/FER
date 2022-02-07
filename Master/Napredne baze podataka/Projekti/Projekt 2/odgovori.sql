1.)
Kako bih osigurala da su regije sličnih veličina prvo sam izračunala površine županija naredbom
SELECT name_1, (ST_Area(geom)) FROM hrv_adm1
Ručno sam formirala grupe županija kako bi bile željene veličine (otprilike 4 jednake cjeline).
Stvorila sam novu tablicu koja sadrži ime regije i njezinu geometriju.
Pomoću ST_Union sam grupirala geometrije županija i stvorila veće regije tipa MultiPolygon.

CREATE TABLE IF NOT EXISTS public.regions
(
    gid integer NOT NULL DEFAULT nextval('hrv_adm1_gid_seq'::regclass),
	name_reg VARCHAR(75) COLLATE pg_catalog."default",
	geom geometry(MultiPolygon,3765),
    CONSTRAINT regions_pkey PRIMARY KEY (gid)
);

INSERT INTO public.regions (name_reg, geom) 
VALUES ('istocna', ST_Multi(ST_Union(array[
	(SELECT geom FROM public.hrv_adm1 WHERE gid=2),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=3),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=12),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=13),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=18),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=19)
]))),
('sredisnja', ST_Multi(ST_Union(array[
	(SELECT geom FROM public.hrv_adm1 WHERE gid=5),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=8),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=9),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=11),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=15),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=17),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=21)
]))),
('zapadna', ST_Multi(ST_Union(array[
	(SELECT geom FROM public.hrv_adm1 WHERE gid=6),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=7),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=10),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=14)
]))),
('juzna', ST_Multi(ST_Union(array[
	(SELECT geom FROM public.hrv_adm1 WHERE gid=1),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=4),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=16),
	(SELECT geom FROM public.hrv_adm1 WHERE gid=20)
])));

Osim ovog načina osiguravanje sličnih veličina regija, površine su se mogle računati i grupirati i u QGIS-u koji ima sposobnost računanja površina.
Grupiranje pomoću QGIS-a te računanje SQL upitima i ručno grupiranje brzi su načini rješavanja problema. 
Za opis zadatka programiranjem ovo je težak problem. Za potpuno programsko rješenje moglo bi se krenuti pristupom dinamičkog programiranja i ispitivanjem optimalnih kombinacija.

--  **** je li tezak???? lol ***



2.)
Kako bi prikazala samo rijeku Savu stvorila sam pogled (view) koji prikazuje sve zapise tablice waterways kojima je ime Sava.
Rijeka je fragmentirana u različite zapise i ovo je način da se izdvoje svi zapisi. 
Upit u ovoj formi ignorira dodatne pritoke i kanale (npr Kanal Sava-Odra).

CREATE VIEW river_sava AS
SELECT *
FROM waterways
WHERE (lower(waterways.name) LIKE 'sava'); 

Duljinu Save u različitim regijama dobila sam tako da sam označila presjek (ST_Intersection) svakog fragmenta Save s regijom i zbrojila duljine presjeka (ST_Length) za svaku regiju (upit ispod).
Duljina rijeke u istočnoj regiji je 83,106 metara, a u središnjoj 219,957 metara. Sava ne prolazi kroz zapadnu i južnu regiju.

SELECT name_reg, round(SUM(ST_Length(ST_Intersection(regions.geom, river_sava.geom))))
FROM regions, river_sava
GROUP BY name_reg


3.)
Kako bi pretraga bila brža stvorila sam indeks nad geometrijom zgrade. Zgrada ima mnogo pa se ovaj postupak isplati, dok fragmenata Save ima samo 11 pa ih nije potrebno indeksirati.

CREATE INDEX buildings_geom_index
    ON public.buildings USING gist
    (geom gist_geometry_ops_nd)
    TABLESPACE pg_default;
	
Zatim je napravljena tablica koja sadrži izračunatu najmanju udaljenost rijeke Save od objekta building i manji fragment rijeke na kojem se dogodi poplava zgrade.
Najbliža zgrada udaljena je 54.25m.
(trajanje: 2min 34sec) 

CREATE TABLE distance_sava AS
SELECT river_sava.gid AS gid, MIN(ST_Distance(river_sava.geom, buildings.geom)) AS distance
FROM river_sava, buildings
GROUP BY river_sava.gid
ORDER BY distance ASC
LIMIT 1

Stvorila sam VIEW Save koji sadrži geometriju Save s minimalnim potrebnim bufferom da dosegne zgradu.
Pri dodavanju buffera koristi se već izračunata vrijednost iz distance_sava umjesto novog računanja.
(trajanje: 186 msec) 

CREATE VIEW river_sava_buffer AS
SELECT river_sava.gid, ST_Buffer(river_sava.geom, (SELECT distance FROM distance_sava)) AS buffergeom	
FROM river_sava

Konačno sam stvorila tablicu koja sadrži gid poplavljene zgrade, gid fragmenta Save na kojem se dogodi poplava, geometriju zgrade i presjek zgrade i izljevene rijeke.
U where dijelu označeno je da se traži zgrada koja ima presjek s (minimalan potrebni izljev + 0.1). 0.1 je dodan zbog zaokruživanja i računanja s float brojevima.
Kako bi se smanjilo vrijeme pretrage dodan je i uvijet da se pretražuje samo fragment Save gdje je pronađen najmanji potrebni izljev.
Tražena zgrada je ona s gid-om 311207.
(trajanje 334 msec) 

CREATE TABLE nearest_flood AS
SELECT DISTINCT buildings.gid AS buildingsgid, river_sava_buffer.gid AS rivergid,
	buildings.geom AS buildingsgeom,
	ST_Intersection(buildings.geom, river_sava_buffer.buffergeom) AS intersectiongeom	
FROM buildings, river_sava_buffer, distance_sava
WHERE ST_Intersects(buildings.geom, ST_Buffer(river_sava_buffer.buffergeom, 0.1) )
AND river_sava_buffer.gid = distance_sava.gid

Korištenje podataka iz baze razdjeljeno je na više koraka (upita) kako bi svaki bio jednostavniji i brži. 
Velik broj podataka se ovim načinom filtrira kako nebi trebali ispitivati dio podataka za koji je već utvrđeno da ne sadrži rezultat.
Koristi se VIEW iz prošlog zadatka kako bi se fokusirali samo na Savu umjesto svih waterways. Tablica distance_sava služi da bi se našla i spremila minimalna udaljenost.
Kako bi se smanjio broj prolaza po velikom broju podataka stvorila sam i tablicu koja sadrži id jedinog fragmenta Save na koji se pretraga fokusira, te minimalnu udaljenost zgrade.
Bez toga željena zgrada bi se tražila po cijeloj duljini Save umjesto na znanom manjem fragmentu. 
Također, ovime se se minimalna udaljenost sprema kao konstanta i više se ne mora računati.

---------------------------------------------------
Na početku sam stvorila tablicu koja prikazuje svaki fragment rijeke Sava i minimalnu udaljenost zgrade od tog fragmenta.
Kroz ovakav pristup odmah se može saznati oko kojeg dijela Save se nalazi najbliža zgrada i koliko je daleko.
Najbliža zgrada udaljena je 54.25m.

CREATE TABLE distance_sava AS (
	SELECT river_sava.gid AS rivergid, MIN(ST_Distance(river_sava.geom, buildings.geom)) AS distance
	FROM river_sava, buildings
	GROUP BY river_sava.gid
)

Kako bi se smanjio broj izvedenih podupita stvara se i privremena tablica river_gid koja označava gid fragmenta Save blizu kojeg je prva poplavljena zgrada.

CREATE TEMP TABLE river_gid AS (
SELECT rivergid 
FROM distance_sava 
WHERE distance = (SELECT MIN(distance) FROM distance_sava))

Slijedi stvaranje tablice koja sadrži konačan odgovor, lokaciju zgrade koja bi prva bila pogođena izljevom, njena udaljenost od rijeke, i lokaciju rijeke proširene za buffer potreban da zgrada bude pogođena.
U where dijelu označeno je da se traži zgrada koja ima presjek s (minimalan potrebni izljev + 0.1). 0.1 je dodan zbog računanja i usporedbi s float brojevima.
Kako bi se smanjilo vrijeme pretrage dodan je i uvijet da se pretražuje samo fragment Save gdje je pronađen najmanji potrebni izljev.
Tražena zgrada je ona s gid-om 311207.

CREATE TABLE nearest_flood AS (
	SELECT DISTINCT buildings.gid AS buildingsgid, river_sava.gid AS rivergid,
	ST_Distance(river_sava.geom, buildings.geom) AS distance,
	buildings.geom AS buildingsgeom, 
	ST_Buffer(river_sava.geom, (SELECT MIN(distance) FROM distance_sava)) AS bufferrivergeom	
FROM buildings, river_sava
WHERE ST_Intersects(buildings.geom, ST_Buffer(river_sava.geom, (SELECT MIN(distance) FROM distance_sava)+0.1))
AND river_sava.gid IN (SELECT * FROM river_gid))

Korištenje podataka iz baze razdjeljeno je na više koraka (upita) kako bi svaki bio jednostavniji. 
Velik broj podataka se ovim načinom filtrira kako nebi trebali ispitivati dio podataka za koji je već utvrđeno da ne sadrže rezultat.
Koristi se VIEW iz prošlog zadatka kako bi se fokusirali samo na Savu umjesto svih waterways. Tablica distance_sava služi da bi se našle i spremile minimalne udaljenosti po fragmentu.
Kako bi se smanjio broj prolaza po velikom broju podataka stvorila sam i TEMP TABLE koja sadrži id jedinog fragmenta Save na koji se pretraga fokusira.
Bez toga udaljenost zgrada bi se računala za cijelu duljinu Save u Hrvatskoj umjesto u samo manjem fragmentu u kojoj je pronađen željeni minimalni rezultat.

----------------------------------------------

bez gid uvjeta 2min 29sek
31 sek
SELECT DISTINCT buildings.*, ST_Distance(river_sava.geom, buildings.geom) 
FROM buildings, river_sava, distance_sava
WHERE ST_Intersects(buildings.geom, ST_Buffer(river_sava.geom, (SELECT MIN(distance) FROM distance_sava)+0.1))
AND river_sava.gid = (SELECT rivergid FROM distance_sava WHERE distance = (SELECT MIN(distance) FROM distance_sava))



SELECT river_sava.gid, MIN(ST_Distance(river_sava.geom, buildings.geom))
FROM river_sava, buildings
GROUP BY river_sava.gid
2min 33s

CREATE INDEX buildings_geom_index
    ON public.buildings USING gist
    (geom gist_geometry_ops_nd)
    TABLESPACE pg_default;
----------------------------------------------

4.)
Kroz SQL upit sam stvorila tablicu forests koja sadrži identifikator svake županije, njezinu geometriju i ukupnu površinu šuma u toj županiji.

CREATE TABLE forests AS
SELECT hrv_adm1.gid AS gid, SUM(ST_Area(ST_Intersection(landuse.geom, hrv_adm1.geom)))/ST_Area(hrv_adm1.geom) AS forestarea,
	hrv_adm1.geom AS countygeom
FROM hrv_adm1, landuse
WHERE landuse.fclass = 'forest'
AND ST_Intersects(hrv_adm1.geom, landuse.geom)
GROUP BY hrv_adm1.gid

Stvorila sam novi sloj u QGIS projektu koji se odnosi na forest tablicu.
Biranju različitih nijansi boje sam pristupila kroz Layer - Layer properties - Symbology.
Kao vrstu simbola odabrala sam categorized i za vrijednosti po kojima se različito bojaju županije uzela atribut forestarea. 
Odabrala sam nove boje (gradijent zelene) i primijenila nove postavke.


5.)
Stvorila sam VIEW cities_30k koji sadrži sve gradove (tablica places, fclass = city, town ili national_capital) s više od 30,000 stanovnika.

CREATE VIEW cities_30k AS
SELECT gid, population, name, geom
FROM places
WHERE population > 30000
AND (fclass = 'town' OR fclass = 'city' OR fclass = 'national_capital')

Za odrediti koje područje gravitira gdje koristila sam Voronoi dijagram. To je dijagram koji razdvaja mapu na plohe koje se sastoje od svih točaka koje gravitiraju pojedinom gradu.
Stvaranje ovog dijagrama je omogućeno geometrijskim alatom "Voronoi polygons" (Vector - Geometry tools - Voronoi polygons).
Kao input layer odabrala sam cities_30k, točke oko kojih će područja gravitirati.
Rezultat alata je novi sloj kojeg se treba učiniti providnim da se vidi karta Hrvatske ispod.



















