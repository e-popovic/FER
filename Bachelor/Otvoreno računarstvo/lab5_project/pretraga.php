<?php
include ("funkcije.php");

$dom = new DOMDocument();
$dom->load("podaci.xml");
$xpath = new DOMXPath($dom);
$xpath->registerNamespace('php','http://php.net/xpath');
$xpath->registerPHPFunctions();

$filter = "/redatelji/osoba".stvoriFilter();
$result = $xpath->query($filter);

?>

<html xmlns="http://www.w3.org/1999/xhtml" lang="hr">

	<head>
		<title>Rezultat pretrage</title>
		<meta charset="utf-8"/>
		<link rel="stylesheet" type="text/css" href="dizajn.css"/>
		<link rel="icon" href="slike/oscar_kipic_prozirni.png"/>
		<script src="detalji.js"></script>
		<link rel="stylesheet" href="https://unpkg.com/leaflet@1.6.0/dist/leaflet.css"
			 integrity="sha512-xwE/Az9zrjBIphAcBb3F6JVqxf46+CDLwfLMHloNu6KEQCAWi6HcDUbeOfBIptF7tcCzusKFjFw2yuvEpDL9wQ=="crossorigin=""/>
		<script src="https://unpkg.com/leaflet@1.6.0/dist/leaflet.js"
			 integrity="sha512-gZwIG9x3wUXg2hdXF6+rVkLF/0Vi9U8D2Ntg4Ga5I5BZpVkVxlJWbSQtXPSiUTtC0TjtGOmxa1AJPuV0CPthew==" crossorigin=""></script>
	</head>

	<body>
		<header>
			<a href="index.html">
			<img  src="slike/oscar_kipic_prozirni.png" alt="nagrada Oscar" id="kipicL" class="kipic lijevi"/>  
			<img  src="slike/oscar_kipic_prozirni.png" alt="nagrada Oscar" id="kipicD" class="kipic desni" />
			</a>
			<h1>Oskarom nagrađeni redatelji</h1>
		</header>
		
		<div class="sastrane">
		<nav>
			<ul>
			<li><a href="index.html" class="navlink">Početna stranica</a></li>
			<li><a href="podaci.xml" class="navlink">Tablica</a></li>
			<li><a href="obrazac.html" class="navlink">Tražilica</a></li>
			<li><a href="https://oscar.go.com/" class="navlink">Službeni web Oscara</a></li>
			<li><a href="http://www.fer.unizg.hr/predmet/or" class="navlink">Otvoreno računarstvo</a></li>
			<li><a href="http://www.fer.unizg.hr" target="_blank" class="navlink">FER</a></li>
			<li><a href="https://www.youtube.com/watch?v=dQw4w9WgXcQ" class="navlink">O nama</a></li>
			<li><a href="mailto:ep50608@fer.hr" class="navlink">Kontakt</a></li>
			</ul>
		</nav>
		<div id="detalji"></div>	<!--mjesto za detalje-->
		</div>
	
		<section>
		<h2>Rezultat pretrage</h2>
		<p>Prikazuju se redatelji koji zadovoljavaju uvjete pretrage i svi njihovi filmovi kojima su osvojili 
		Oscara za najboljeg redatelja.<br/>Uz svakog redatelja prikazani su slika, mjesto rođenja i akcija. 
		Pritiskom na akciju (povećalo) sa strane se prikazuju redateljevi oskarovski filmovi i njihov žanr. 
		Ispod tablice prikazuje se karta s redateljevim mjestom rođenja. Na karti su označene koordinate
		mjesta dobivene različitim API-jima.<br/>		
		Za svaki film dostupna je ocjena sa stranice <a href="http://imdb.com">IMDb.com</a>. 
		Pritiskom na tekst <i>Prikaži više</i> dobiva se kratak sadržaj filma na engleskom.</p>
		<table class="podaci">
			<tr>
				<th>Ime</th>
				<th>Slika</th>
				<th>Rodno mjesto</th>
				<th>Akcija</th>
				<th>Film</th>
				<th>IMDb ocjena</th>
				<th>Kratki sadržaj</th>
				
			</tr>
			<?php
			 //$prije= round(microtime(true) * 1000);
			 
			 $adr = dajAdresu($result);
			
				foreach($result as $node){
					/*promjena ne radi ako originalnu boju tablice definiram u external css*/
					echo '<tr style="background-color:#d9d9d9;" 
					onmouseover="promijeniBoju(this)" onmouseout="vratiBoju(this)"><td>';
					$ime= ($node->getElementsByTagName('ime')->item(0)->nodeValue);
					$punoime=$ime." ".($node->getElementsByTagName('prezime')->item(0)->nodeValue);
					echo $punoime;
					echo "</td><td>";
				
					$slikaisazetak=dajSlikuISazetak($node);
					echo "<img src=";
					echo $slikaisazetak[0];
					echo " alt='slika redatelja' class='slika_redatelja' />";
					echo "</td><td>";
					
					$adresa=strval($adr[$ime]);
					echo $adresa;
					echo "</td><td>";
					
					// !URL mijenjati ovisno o lokaciji datoteke!
					$url = "http://localhost/0036506085_Popovic_Ema_5/detalji.php?id="."".$node->getAttribute("id")."&show=simple";
					$koord1=dajKoordinate1($node);
					$koord1=explode(", ", $koord1);
					$koord2=dajKoordinate2($adresa);
					$koord2=explode(", ", $koord2);
					echo '<img src="slike/povecalo.png" onclick="dajMapu( \'';
					echo $punoime;
					echo '\' ,';
					echo $koord1[0];
					echo ',';
					echo $koord1[1];
					echo ',';
					echo $koord2[0];
					echo ',';
					echo $koord2[1];
					echo ', \'';
					echo $adresa;
					echo '\');dajDetalje(\'';
					echo $url;
					echo '\');" alt="povecalo" id="povecalo"/>';
					echo "</td><td>";
					
					$film=$node->getElementsByTagName('film');
					$ofilmu=dajInfoOFilmu($film->item(0));
					echo ($film->item(0)->getElementsByTagName('naslov')->item(0)->getElementsByTagName('hrnaslov')->item(0)->nodeValue);
					echo "</td><td>";
					
					echo $ofilmu[0];
					echo "/10</td><td>";
					
					echo '<p onclick="dajSadrzaj(this, \'';	
					echo $ofilmu[1];
					echo '\')"><i>Prikaži više.</i></p>';
					
					if (count($film) == 2){
						$ofilmu=dajInfoOFilmu($film->item(1));
						echo "<tr style='background-color:#d9d9d9;'
							onmouseover='promijeniBoju(this)' onmouseout='vratiBoju(this)'><td></td><td></td><td><td></td><td>";
						echo ($film->item(1)->getElementsByTagName('naslov')->item(0)->getElementsByTagName('hrnaslov')->item(0)->nodeValue);
						echo "</td><td>";
						
						echo $ofilmu[0];
						echo "/10</td><td>";
						
						echo '<p onclick="dajSadrzaj(this, \'';	
						echo $ofilmu[1];
						echo '\')"><i>Prikaži više.</i></p>';
						echo "</td></tr>";
					}
				}
				//$poslje= round(microtime(true) * 1000);
				//echo ($poslje-$prije);
				//echo " ms";
			?>
		</table><br/>
		
		<div id="mapid" style="display:none;">
		</div>	<!--mjesto za mapu-->
		
		</section>
		
		<footer>
			<p>Autor: Ema Popović<br/>
			Fakultet elektrotehnike i računarstva, Zagreb<br/>
			Zadnja izmjena: 2020-05-27</p>
		</footer>

	</body>

</html>