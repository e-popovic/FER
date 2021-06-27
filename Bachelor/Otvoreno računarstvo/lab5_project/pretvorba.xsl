<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="xml" indent="yes" doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd" doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN"/>
	<xsl:template match="/">
		<html xmlns="http://www.w3.org/1999/xhtml" lang="hr">
			<head>
				<title>Tablica</title>
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
				</div>
				
				<section>
				<h2>Tablica</h2>
				<p>U ovoj tablici nalazi se popis Oscarom nagrađenih redatelja 21. stoljeća. Za svakog redatelja navedene su godina i država rođenja.
				Uz njih stoje i filmovi koji su im osvojili ovu prestižnu nagradu. Za svaki film može se provjeriti ime, godina, žanr i je li film osvojio Oscara za najbolji film.</p>
				<table id="usredini" class="podaci">
					<tr>
						<th>Ime</th>
						<th>Godina rođenja</th>
						<th>Država rođenja</th>
						<th>Ime filma</th>
						<th>Godina</th>
						<th>Žanr</th>
						<th>Najbolji film</th>
					</tr>
					<xsl:for-each select="redatelji/osoba">
					<xsl:sort select="film[1]/godinaf"/>
					<tr>
						<td><xsl:value-of select="ime" /><xsl:text> </xsl:text><xsl:value-of select="prezime" /></td>
						<td><xsl:value-of select="godinar" /></td>
						<td><xsl:value-of select="drzavarod" /></td>
						<td><xsl:value-of select="film[1]/naslov/hrnaslov" /></td>
						<td><xsl:value-of select="film[1]/godinaf" /></td>
						<td>
						<xsl:for-each select="film[1]/zanr">
						<xsl:sort select="."/>
							<xsl:choose>
								<xsl:when test="position() = last()">
									<xsl:value-of select="." />
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="." />, 
								</xsl:otherwise>
							</xsl:choose>
						</xsl:for-each>
						</td>
						<td><xsl:if test="film[1]/najbolji">
							<img  src="slike/kvacica.png" alt="zelena kvacica"  id="kvacica"/>
						</xsl:if></td>
						
					</tr>
					<xsl:if test="film[2]">
					<tr>	
						<td></td><td></td><td></td>
						<td><xsl:value-of select="film[2]/naslov/hrnaslov" /></td>
						<td><xsl:value-of select="film[2]/godinaf" /></td>
						<td><xsl:for-each select="film[2]/zanr">
						<xsl:sort select="."/>
							<xsl:choose>
								<xsl:when test="position() = last()">
									<xsl:value-of select="." />
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="." />, 
								</xsl:otherwise>
							</xsl:choose>
						</xsl:for-each></td>
						<td><xsl:if test="film[2]/najbolji">
							<img  src="slike/kvacica.png" alt="zelena kvacica"  id="kvacica"/>
						</xsl:if></td>
					</tr>
					</xsl:if>
					</xsl:for-each>
					
				</table>
				</section>
				
				<footer>
					<p>Autor: Ema Popović<br/>
					Fakultet elektrotehnike i računarstva, Zagreb<br/>
					Zadnja izmjena: 2020-04-13</p>
				</footer>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>