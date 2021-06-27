<?php
$dom = new DOMDocument();
$dom->load("podaci.xml");
$xpath = new DOMXPath($dom);
$xpath->registerNamespace('php','http://php.net/xpath');
$xpath->registerPHPFunctions();

//formiranje pretrage kroz xml
function stvoriFilter(){
	$filter[]=array();
	$i=0;
	
	//contains -- prvo iz xml, drugo iz trazilice
	if(isset($_REQUEST['ime'])){
		$filter[$i] = "[ime[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($_GET['ime'])."')]]";	
		$i++;
	}
	
	if(isset($_REQUEST['prezime'])){
		$filter[$i] = "[prezime[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($_GET['prezime'])."')]]";	
		$i++;
	}
	
	if(isset($_REQUEST['godinar'])){
		if ($_GET['godinar'] != "odaberi"){
			$filter[$i] = "[godinar[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($_GET['godinar'])."')]]";	
			$i++;
		}
	}
	
	if(isset($_REQUEST['govpodrucje'])){
		if ($_GET['govpodrucje'] == 'englesko'){
			$filter[$i] = "[drzavarod/@govpodrucje='englesko']";
			$i++;
		}
		else {
			$filter[$i] = "[drzavarod/@govpodrucje='drugo']";
			$i++;
		}
	}
	
	if(isset($_REQUEST['mjestorod'])){
		$filter[$i] = "[mjestorod[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($_GET['mjestorod'])."')]]";	
		$i++;
	}
	
	if(isset($_REQUEST['drzavarod'])){
		$filter[$i] = "[drzavarod[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($_GET['drzavarod'])."')]]";	
		$i++;
	}
	
	if(isset($_REQUEST['nagrade'])){
		$filter[$i]="[nagrade[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($_GET['nagrade'])."')]]";
		$i++;
	}
	
	if(isset($_REQUEST['nominacije'])){
		$filter[$i]="[nominacije[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($_GET['nominacije'])."')]]";
		$i++;
	}
	
	if(isset($_REQUEST['wiki'])){
		$filter[$i] = "[wiki[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($_GET['wiki'])."')]]";	
		$i++;
	}
	
	if(isset($_REQUEST['id'])){
		$filter[$i] = "[contains(@id,'".mb_strtolower($_GET['id'])."')]";	
		$i++;
	}
	
	if(isset($_REQUEST['hrnaslov'])){
		$filter[$i] = "[film/naslov/hrnaslov[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($_GET['hrnaslov'])."')]]";	
		$i++;
	}
	
	if(isset($_REQUEST['engnaslov'])){
		$filter[$i] = "[film/naslov/engnaslov[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($_GET['engnaslov'])."')]]";	
		$i++;
	}
	
	if(isset($_REQUEST['nije_eng'])){
		if(isset($_REQUEST['izvnaslov'])){
			$filter[$i] = "[film/naslov/izvnaslov[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($_GET['izvnaslov'])."')]]";	
			$i++;
		}
	}
	
	if(isset($_REQUEST['godinaf'])){
		if ($_GET['godinaf'] != "odaberi"){
			$filter[$i] = "[film/godinaf[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($_GET['godinaf'])."')]]";	
			$i++;
		}
	}
	
	if(isset($_REQUEST['najbolji'])){
		$filter[$i] = "[film[boolean(najbolji)]]";	
		$i++;
	}
	
	if(isset($_REQUEST['jezik'])){
		$filter[$i] = "[film/jezik[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($_GET['jezik'])."')]]";	
		$i++;
	}
	
	if(isset($_REQUEST['zanr'])){
		$zanrovi=$_REQUEST['zanr'];
		$brzanrova=count($zanrovi);
		$filter[$i] = "[film[zanr[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($zanrovi[0])."')]]";
		for ($x=1; $x<$brzanrova; $x++){
			$filter[$i].=" and film[zanr[contains(php:functionString('mb_strtolower',text()),'".mb_strtolower($zanrovi[$x])."')]]";
		}
		$filter[$i].="]";
		
		$i++;
	}
	
	//prebaci ih sve u jedan filter string
	$sfilter="";
	for ($x=0; $x<$i; $x++){
		$sfilter.=$filter[$x];
	}
	return $sfilter;
}

//vrati link na sliku s wikipedije
function dajSliku($redatelj){
	//Wikimedia REST API
	$zahtjev = "https://en.wikipedia.org/api/rest_v1/page/summary/".($redatelj->getElementsByTagName('wiki')->item(0)->nodeValue);
	if ($jsonfile = file_get_contents($zahtjev)){
		$wikirezultat = json_decode($jsonfile,true);
		$wikirezultat = $wikirezultat["originalimage"]["source"];
	}
	else
		$wikirezultat="Nije pronađeno.";
	return $wikirezultat;
}

//vrati koordinate mjesta rodenja s wikipedije(1)
/*!!!!! LAB4 NAPOMENA: Wikimedia REST API ne prikazuje koordinate za Wiki stranice ljudi. Zato za prikaz koordinata redateljevog mjesta rodenja
		pomocu Wikimedia REST API-ja koristim Wiki stranicu njegovog/njenog rodnog mjesta. Postupak s podacima iz API-ja je jednak.
		Zbog toga postoji element wikimjesto s wiki handle-om mjesta. Isti je kao element mjestorod, samo sa engleskim nazivom, pa nema
		dodatne pretrage po njemu. */
function dajKoordinate1($mjesto){
	//Wikimedia REST API
	$zahtjev = "https://en.wikipedia.org/api/rest_v1/page/summary/".($mjesto->getElementsByTagName('wikimjesto')->item(0)->nodeValue);
	if ($jsonfile = file_get_contents($zahtjev)){
		$wikirezultat = json_decode($jsonfile,true);
		$koordinate="";
		$wikirezultat=$wikirezultat["coordinates"];
		$koordinate .= number_format($wikirezultat["lat"],2);
		$koordinate .= ", ";
		$koordinate .= number_format($wikirezultat["lon"],2);
	}
	else
		$koordinate="Nije pronađeno.";
	return $koordinate;
}

//vrati sazetak s wikipedije
function dajSazetak($redatelj){
	//Wikimedia REST API
	$zahtjev = "https://en.wikipedia.org/api/rest_v1/page/summary/".($redatelj->getElementsByTagName('wiki')->item(0)->nodeValue);	
	if ($jsonfile = file_get_contents($zahtjev)){
		$wikirezultat = json_decode($jsonfile,true);
		$wikirezultat = $wikirezultat["extract"];
		$wikirezultat = strlen($wikirezultat) > 50 ? substr($wikirezultat,0,50)."..." : $wikirezultat;	//skracuje opis
	}
	else
		$wikirezultat="Nije pronađeno.";
	return $wikirezultat;
}

//vrati mjesto rodenja s wikipedije
//pod pages vise kljuceva kad |
function dajAdresu($redatelj){
	//MediaWiki Action API
	$zahtjev = "https://en.wikipedia.org/w/api.php?action=query&prop=revisions&rvprop=content&rvsection=0&titles=".($redatelj->getElementsByTagName('wiki')->item(0)->nodeValue)."&format=json";
	
	if ($jsonfile = file_get_contents($zahtjev)){
		$wikirezultat = json_decode($jsonfile,true);
	
		//iskopaj blok teksta s lokacijom
		$wikirezultat=$wikirezultat["query"]["pages"];
		$wikirezultat=$wikirezultat[array_key_first($wikirezultat)];
		$wikirezultat=$wikirezultat["revisions"]["0"]["*"];
	
		//iskopaj lokaciju iz bloka teksta
		$indeks=strpos($wikirezultat, "birth_place");
		$mjestorod=explode("|",substr($wikirezultat,$indeks));
		$mjestorod=$mjestorod[0];
		$mjestorod=explode(" = ",$mjestorod);
		$mjestorod=$mjestorod[1];
		$mjestorod=str_replace("U.S.","US",$mjestorod);
		$mjestorod=str_replace("[","",$mjestorod);
		$mjestorod=str_replace("]","",$mjestorod);
		if ($usindeks=strpos($mjestorod, "US")){
			$mjestorod=substr($mjestorod, 0, $usindeks+2);
		}
		
		//iznimke...
		if ( ($redatelj->getElementsByTagName('prezime')->item(0)->nodeValue)=="Coen")
			$mjestorod=str_replace(" (both)","",$mjestorod);
		else if ( ($redatelj->getElementsByTagName('prezime')->item(0)->nodeValue)=="Lee")
			$mjestorod.=", Taiwan";
		else if ( ($redatelj->getElementsByTagName('prezime')->item(0)->nodeValue)=="Boyle")
			$mjestorod.=", England";
		
		return $mjestorod;
	}
	else{
		return "Nije pronađeno.";
	}
}

//vrati koordinate mjesta rodenja s wikipedije(2) - drugi api
function dajKoordinate2($mjestorod){
	//OpenStreetMap Nominatim
	$koordinate="";
	$mjestorod=str_replace(" ","%20",$mjestorod);
	
	$zahtjev = "https://nominatim.openstreetmap.org/search?q=".$mjestorod."&format=xml";
	//iduce linije potrebne, set-anje user agenta! - bez njih baca error 403 
	$options  = array('http' => array('user_agent' => 'custom user agent'));
	$context  = stream_context_create($options);
	
	
	if ($xmlrezultat = file_get_contents($zahtjev, false, $context)){
		$xml = simplexml_load_string($xmlrezultat);
		if($xml->place &&  $xml->place->attributes()) {
			
			$lat= $xml->place['lat'];
			$lon= $xml->place['lon'];
			$koordinate.=number_format((float)$lat,2).", ".number_format((float)$lon,2);
		}
		else{
			$koordinate.="Nije pronađeno.";
		}
	}
	else{
		$koordinate.="Nije pronađeno.";
	}
	return $koordinate;
}                                  

?>