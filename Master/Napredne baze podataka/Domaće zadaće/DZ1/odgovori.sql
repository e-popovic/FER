1.)
SELECT *, rank() OVER (ORDER BY noOfTracks DESC) AS rang
FROM
(SELECT DISTINCT castmember, COUNT(*) AS noOfTracks
FROM track NATURAL JOIN 
	(SELECT trackid, UNNEST (castandcrew) AS castmember
		FROM track) castlist
WHERE castmember LIKE 'Kate %'	
GROUP BY castmember
ORDER BY noOfTracks DESC) allKates


2.)
WITH RECURSIVE zanrBroj (genreid, genrename, trackid, supgenreid)
AS (
	(SELECT genreid, genrename, trackid, supgenreid
	FROM movie NATURAL JOIN trackgenre NATURAL JOIN genre)
	UNION
	(SELECT genre.genreid, genre.genrename, zanrBroj.trackid, genre.supgenreid
	FROM zanrBroj, genre
	WHERE zanrBroj.supgenreid = genre.genreid)
)
SELECT genreid, genrename, COUNT(zanrBroj.trackid) AS numofmovies
FROM zanrBroj
GROUP BY genreid, genrename
ORDER BY genreid;


3.)
SELECT tracktitle, viewstartdatetime::time, 
	COUNT(*) OVER (PARTITION BY tracktitle
	ORDER BY viewstartdatetime::time
	RANGE BETWEEN '4 h'::INTERVAL PRECEDING AND CURRENT ROW) currentandprev4hours
FROM showep NATURAL JOIN track NATURAL JOIN trackview
NATURAL JOIN (SELECT trackid, 
	UNNEST (string_to_array(tracktitle, ' ')) showtracktitle
	FROM track) showtrack
WHERE levenshtein(lower(showtrack.showtracktitle), 'vidaut') <=3
ORDER BY tracktitle


4.)
SELECT tracktitle, director[1] AS firstdirector 
FROM track
WHERE director[1] IS NOT NULL
AND strict_word_similarity('colect', tracktitle) > 0.3;


5.)
CREATE TEMP TABLE zanrovi (zanrime VARCHAR);
INSERT INTO zanrovi VALUES ('Thrillers'),('Family Movies'),('Horror'),('Stand-Up Comedy'),('Sports Movies');

SELECT *
FROM crosstab('SELECT (owner.lastname || '' '' || owner.firstname) AS firstandlastname, genrename, COUNT(*) AS brojlajkova
				FROM owner NATURAL JOIN profiletrack NATURAL JOIN track NATURAL JOIN trackgenre NATURAL JOIN genre
				WHERE (genrename LIKE ''Thrillers'' OR genrename LIKE ''Family Movies'' OR genrename LIKE ''Horror'' OR genrename LIKE ''Stand-Up Comedy'' OR genrename LIKE ''Sports Movies'')
				AND similarity(owner.firstname, ''Leon'') >= 0.2
                AND profiletrack.liked IS NOT NULL
				GROUP BY firstandlastname, genrename
				ORDER BY firstandlastname, genrename', 
			  'SELECT zanrime FROM zanrovi ORDER BY zanrime')
AS pivotTable (ownername VARCHAR, family_movies INT, horror INT, sports_movies INT, stand_up_comedy INT, thrillers INT)
ORDER BY ownername;