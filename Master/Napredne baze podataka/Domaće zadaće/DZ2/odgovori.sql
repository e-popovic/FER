1.)
SELECT landuse.gid, ST_Area(landuse.geom)
FROM landuse, muny
WHERE landuse.fclass = 'residential'
AND muny.name_2 = 'Vis'
AND ST_Within(landuse.geom, muny.geom)
AND landuse.gid IN (
    SELECT parcela.gid
    FROM landuse AS parcela, landuse AS zelena
    WHERE parcela.gid = landuse.gid
    AND (zelena.fclass = 'forest' OR zelena.fclass = 'grass' OR zelena.fclass = 'park')
    AND ST_Touches(parcela.geom, zelena.geom)
)
AND landuse.gid NOT IN (
    SELECT parcela.gid
    FROM landuse AS parcela, landuse AS zelena
    WHERE parcela.gid = landuse.gid
    AND NOT (zelena.fclass = 'forest' OR zelena.fclass = 'grass' OR zelena.fclass = 'park')
    AND ST_Touches(parcela.geom, zelena.geom)
)


2.)
SELECT buildings.gid, round(ST_Distance(buildings.geom, water.geom)) as distance, round(ST_Area(buildings.geom)) AS area  
FROM buildings, water
WHERE water.name = 'Crveno jezero'
AND ST_CoveredBy(buildings.geom, ST_Buffer(water.geom, 1000))
ORDER BY area DESC  
LIMIT 5;


3.)
WITH udaljenosti AS (
	SELECT landuse.name AS forest, round(min(ST_Distance(landuse.geom, buildings.geom))) AS dist, buildings.name AS firedpt,
	rank() OVER (PARTITION BY landuse.name ORDER BY round(min(ST_Distance(landuse.geom, buildings.geom)))) AS rg
	FROM landuse, buildings
	WHERE landuse.fclass = 'forest'
	AND (LOWER(buildings.name) LIKE '%dvd%' OR LOWER(buildings.name) LIKE '%vatrog%')
	AND landuse.name IS NOT NULL
	AND ST_Distance(landuse.geom, buildings.geom) < 5000
	GROUP BY landuse.name, buildings.name)
SELECT forest, dist, firedpt
FROM udaljenosti
WHERE rg = 1



4.)
WITH udaljenosti AS (
	SELECT buildings.name, water.name, round(ST_Distance(buildings.geom, water.geom)) AS dist,
	rank() OVER (PARTITION BY buildings.name ORDER BY round(ST_Distance(buildings.geom, water.geom)) ASC) AS rn
	FROM buildings, water
	WHERE water.fclass = 'river'
	AND (lower(buildings.name) LIKE '%vatrog%'
	OR lower(buildings.name) LIKE '%dvd%'))
SELECT *
FROM udaljenosti
WHERE (rn = 1 OR rn = 2)


5.)
WITH udaljenosti AS (
	SELECT nature.name AS ime, round(ST_Distance(nature.geom, pois.geom)) AS dist,
	rank() OVER (PARTITION BY nature.name ORDER BY round(ST_Distance(nature.geom, pois.geom)) ASC) AS rn
	FROM nature, pois
	WHERE nature.fclass = 'beach'
	AND pois.fclass = 'bar')
SELECT ime, dist
FROM udaljenosti
WHERE rn = 1

--isto radi
select nature.name, round(min(st_distance(nature.geom,pois.geom)))
from nature, pois
where nature.fclass like 'beach' and pois.fclass like 'bar' 
group by nature.name
order by nature.name asc