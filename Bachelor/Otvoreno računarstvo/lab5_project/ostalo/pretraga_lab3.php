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
	</head>

	<body>
		<header>
			<a href="index.html">
			<img  src="slike/oscar_kipic_prozirni.png" alt="nagrada Oscar" id="kipicL" class="kipic lijevi"/>  
			<img  src="slike/oscar_kipic_prozirni.png" alt="nagrada Oscar" id="kipicD" class="kipic desni" />
			</a>
			<h1>Oskarom nagrađeni redatelji</h1>
		</header>

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
		
		<section>
		<h2>Rezultat pretrage</h2>
		<p>Prikazuju se redatelji koji zadovoljavaju uvjete pretrage i svi njihovi filmovi kojima su osvojili Oscar za najboljeg redatelja.</p>
		<table class="podaci">
			<tr>
				<th>Ime</th>
				<th>Godina rođenja</th>
				<th>Država rođenja</th>
				<th>Ime filma</th>
				<th>Godina</th>
				<th>Žanr</th>
				<th>Najbolji film</th>
			</tr>
			<?php
			//var_dump($filter);

				foreach($result as $node){
					
					
					echo "<tr><td>";
					echo ($node->getElementsByTagName('ime')->item(0)->nodeValue)."&nbsp;".($node->getElementsByTagName('prezime')->item(0)->nodeValue);
					echo "</td><td>";
					echo ($node->getElementsByTagName('godinar')->item(0)->nodeValue);
					echo "</td><td>";
					echo ($node->getElementsByTagName('drzavarod')->item(0)->nodeValue);
					echo "</td><td>";
					$film=$node->getElementsByTagName('film');
					echo ($film->item(0)->getElementsByTagName('naslov')->item(0)->getElementsByTagName('hrnaslov')->item(0)->nodeValue);
					echo "</td><td>";
					echo ($film->item(0)->getElementsByTagName('godinaf')->item(0)->nodeValue);
					echo "</td><td>";
					$brzanr= count($film->item(0)->getElementsByTagName('zanr'));
					for ($i=0; $i< $brzanr-1; $i++){
						echo ($film->item(0)->getElementsByTagName('zanr')->item($i)->nodeValue).",&nbsp;";
					}
					echo ($film->item(0)->getElementsByTagName('zanr')->item($brzanr-1)->nodeValue);
					echo "</td><td>";
					if (isset($film->item(0)->getElementsByTagName('najbolji')->item(0)->nodeValue)){
						echo '<img src="slike/kvacica.png" alt="zelena kvacica"  id="kvacica"/>';
					}
					
					echo "</td></tr>";
					//ak ima jos jedan film
					
					if (count($film) == 2){
						echo "<tr><td></td><td></td><td></td><td>";
						echo ($film->item(1)->getElementsByTagName('naslov')->item(0)->getElementsByTagName('hrnaslov')->item(0)->nodeValue);
						echo "</td><td>";
						echo ($film->item(1)->getElementsByTagName('godinaf')->item(0)->nodeValue);
						echo "</td><td>";
						$brzanr= count($film->item(1)->getElementsByTagName('zanr'));
						for ($i=0; $i< $brzanr-1; $i++){
							echo ($film->item(1)->getElementsByTagName('zanr')->item($i)->nodeValue).",&nbsp;";
						}
						echo ($film->item(1)->getElementsByTagName('zanr')->item($brzanr-1)->nodeValue);
						echo "</td><td>";
						if (isset($film->item(1)->getElementsByTagName('najbolji')->item(0)->nodeValue)){
							echo '<img src="slike/kvacica.png" alt="zelena kvacica"  id="kvacica"/>';
						}
						echo "</td></tr>";
					}	
				}
			?>
		
		</table>
		</section>

		<footer>
			<p>Autor: Ema Popović<br/>
			Fakultet elektrotehnike i računarstva, Zagreb<br/>
			Zadnja izmjena: 2020-04-25</p>
		</footer>
	</body>

</html>