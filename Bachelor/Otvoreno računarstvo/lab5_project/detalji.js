/*mijenja boju retka preko kojeg se prelazi misem*/
function promijeniBoju(moj_red_tablice){
	moj_red_tablice.style.backgroundColor = "#f4eabe"
}

/*mijenja boju retka nakon prelaska misem*/
function vratiBoju(moj_red_tablice){
	moj_red_tablice.style.backgroundColor = "#d9d9d9"
}

/*daje filmove zeljenog redatelja*/
var req; 
function dajDetalje(url){
	//gif s wikipedije: http://en.wikipedia.org/wiki/Image:Spinning_wheel_throbber.gif
	document.getElementById("detalji").innerHTML= "<img src='slike/loading.gif' alt='loading'/>";
	if (window.XMLHttpRequest) { // FF, Safari, Opera, IE7+
		req = new XMLHttpRequest(); // stvaranje novog objekta
	} 
	else if (window.ActiveXObject) { // IE 6+
		req = new ActiveXObject("Microsoft.XMLHTTP"); //ActiveX
	}
	
	if (req) { // uspješno stvoren objekt
		req.onreadystatechange = doSomething;
		req.open("GET", url, true); // metoda, URL, asinkroni način
		req.send(null); //slanje (null za GET, podaci za POST)
	}
}
/*pomaze funkciji dajDetalje*/
function doSomething () {
	if (req.readyState == 4) { // primitak odgovora
		if (req.status == 200) { // kôd statusa odgovora = 200 OK
			document.getElementById("detalji").innerHTML=req.responseText;
		} 
		else { // kôd statusa nije OK
			alert("Nije primljen 200 OK, nego:\n" + req.statusText);
		}
	}
}

/*stvara mapu s mjestom rodenja redatelja -> Leaflet.js*/
function dajMapu(ime, lat1, lon1, lat2, lon2, adresa){
	document.getElementById("mapid").style.display = "block";
	document.getElementById("mapid").innerHTML= "<img src='slike/loading.gif' alt='loading'/>";
	
	var mymap = L.map('mapid').setView([lat1, lon1], 15);
	
	//pozadina
	var mylayer = L.tileLayer('https://api.mapbox.com/styles/v1/{id}/tiles/{z}/{x}/{y}?access_token=pk.eyJ1IjoibWFwYm94IiwiYSI6ImNpejY4NXVycTA2emYycXBndHRqcmZ3N3gifQ.rJcFIG214AriISLbB6B5aw', {
		maxZoom: 18,
		attribution: 'Map data &copy; <a href="https://www.openstreetmap.org/">OpenStreetMap</a> contributors, ' +
			'<a href="https://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, ' +
			'Imagery © <a href="https://www.mapbox.com/">Mapbox</a>',
		id: 'mapbox/streets-v11',
		tileSize: 512,
		zoomOffset: -1
	});
	mylayer.addTo(mymap);
	
	//markeri lokacija
	var marker1 = L.marker([lat1, lon1]).addTo(mymap);
	marker1.bindPopup(ime+"<br/>Širina: "+String(Math.round((lat1 + Number.EPSILON) * 1000) / 1000)+
		"<br/>Dužina: "+String(Math.round((lon1 + Number.EPSILON) * 1000) / 1000)
		+"<br/>"+adresa+"<br/>Wikimedia REST API").openPopup();
	var marker2 = L.marker([lat2,lon2]).addTo(mymap);
	marker2.bindPopup(ime+"<br/>Širina: "+String(Math.round((lat2 + Number.EPSILON) * 1000) / 1000)+
		"<br/>Dužina: "+String(Math.round((lon2 + Number.EPSILON) * 1000) / 1000)
		+"<br/>"+adresa+"<br/>MediaWiki Action API").openPopup();
	
	//linija izmedu markera
	var latlngs = [
    [lat1, lon1],
    [lat2, lon2]
	];
	var polyline = L.polyline(latlngs, {color: 'red'}).addTo(mymap);
}

/*ispisuje kratki sadrzaj filma*/
function dajSadrzaj(id, tekst){
	id.innerHTML=tekst;
}