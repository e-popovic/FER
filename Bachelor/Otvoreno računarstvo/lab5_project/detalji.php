<?php
include ("funkcije.php");		//?

$moj_id=$_GET["id"];

$dom = new DOMDocument();
$dom->load("podaci.xml");
$xpath = new DOMXPath($dom);

//$filter = "/redatelji/osoba[contains(@id,'".$moj_id."')]";
$filter = "/redatelji/osoba[@id='".$moj_id."']";
$result = $xpath->query($filter);

$izlaz="";
$pomocni="";
foreach($result as $redatelj){
	
	if(($moj_id) == ($redatelj->getAttribute('id'))){
	
	//($redatelj->getElementsByTagName('ime')->item(0)->nodeValue);
	$pomocni="<p><strong>".($redatelj->getElementsByTagName('ime')->item(0)->nodeValue);	//ime
	$pomocni.=" ".($redatelj->getElementsByTagName('prezime')->item(0)->nodeValue)."</strong></p>";	//prezime
	$filmovi=$redatelj->getElementsByTagName('film');
	$pomocni.= "<dl>";
	foreach($filmovi as $film){	// naslov (godina) - za svaki redateljev film
		$pomocni.= "<dt>-> ".($film->getElementsByTagName('naslov')->item(0)->getElementsByTagName('hrnaslov')->item(0)->nodeValue);
		$pomocni.= " (".($film->getElementsByTagName('godinaf')->item(0)->nodeValue).")</dt>";
		
		$zanrovi=$film->getElementsByTagName('zanr');
		foreach($zanrovi as $zanr){
			$pomocni.= "<dd>- ".($zanr->nodeValue)."</dd>";
		}
	}
	$pomocni.= "</dl>";
	$izlaz.=$pomocni;
}
}
echo $izlaz;

?>