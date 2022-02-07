rv.films = rv.films.filter((obj, idx, arr) => (
	idx === rv.films.findIndex((o) => (o.title === obj.title && o.actors === obj.actors))
	//rv.films.findIndex((o)=>o === obj) === idx
));



// vrati prosjecni broj laur u svakoj kategoriji
db.nobelprizes.mapReduce(
`function() {
    if (this.laureates !== undefined){
        emit(
            this.category,
            { noLaureates: this.laureates.length, cntLaureates: 1 }
        );
    };
};`,
`function(key, values) {
    rv = { noLaureates: 0, cntLaureates: 0 };
    values.forEach( function(value) {
        rv.noLaureates = rv.noLaureates + value.noLaureates;
        rv.cntLaureates = rv.cntLaureates + value.cntLaureates;
    });
    return rv;
    
};`,
{ finalize: `function(key, rv) {
    return { avg: (rv.noLaureates/rv.cntLaureates).toFixed(2) };
    
};`,
out: {inline:1}
}
);

// za svaki sat vrati koliko je filmova pogledano i po kategoriji. samo ako ima samo jednu kategoriju
db.dvdrent.mapReduce(
`function() {
    if (this.film != undefined && this.film.categories.length == 1) {
        var hour = new Date(this.rental_date).getHours();
        categs = {};
        categs[this.film.categories[0].name] = 1;
        emit(
            {hour: hour},
            {categories: categs, films: 1}
        );
    }
};`,
`function(key, values) {
    rv = {categories: {}, films: 0};
    values.forEach( function(value) {
        rv.films += value.films;
        for (var cat in value.categories){
           if (rv.categories[cat] !== undefined) {
                rv.categories[cat] += value.categories[cat];
            }
            else {
                rv.categories[cat] = value.categories[cat];
            } 
        }
    });
    return rv;
};`,
{
    out: {inline:1}
}
);

// karte, za svako pojavljivanje battlefield, life, death ispisi sve razlicite varijante i broj pojavljivanja
db.cards.mapReduce(
`function() {
    if (this.text !== undefined) {
        var mytext = this.text;
        var k1 = "battlefield";
        var k2 = "death";
        var k3 = "life";
        var regex_1 = /battlefield/gi;
        var regex_2 = /death/gi;
        var regex_3 = /life/gi;
        var ar1 = [];
        var ar2 = [];
        var ar3 = [];
        var c1 = 0;
        var c2 = 0;
        var c3 = 0;        
        
        var result1, indices1 = [];
        while ( (result1 = regex_1.exec(mytext)) ) {
            indices1.push(result1.index);
        }    
        if (indices1.length > 0) {
            indices1.forEach( function(ind) {
                c1 += 1;
                x = ar1.push(mytext.substring(ind, ind+11));
            });
            emit(
                k1,
                {cnt: c1, variations: ar1}
            );
        }
        
        var result2, indices2 = [];
        while ( (result2 = regex_2.exec(mytext)) ) {
            indices2.push(result2.index);
        }    
        if (indices2.length > 0) {
            indices2.forEach( function(ind) {
                c2 += 1;
                x = ar2.push(mytext.substring(ind, ind+5));
            });
            emit(
                k2,
                {cnt: c2, variations: ar2}
            );
        }
        
        var result3, indices3 = [];
        while ( (result3 = regex_3.exec(mytext)) ) {
            indices3.push(result3.index);
        }    
        if (indices3.length > 0) {
            indices3.forEach( function(ind) {
                c3 += 1;
                x = ar3.push(mytext.substring(ind, ind+4));
            });
            emit(
                k3,
                {cnt: c3, variations: ar3}
            );
        }
    }
};`,
`function(key, values) {
    rv = {cnt: 0, variations: []};
    values.forEach( function(value) {
        rv.cnt += value.cnt;
        rv.variations = rv.variations.concat(value.variations);
    });
    return rv;
};`,
{
    finalize: `function(key, rv) {
        rv.variations = rv.variations.filter((obj, idx, arr)=>(
            arr.findIndex((o) => o === obj) === idx
        ));
        
        rv.variations.sort(function(a,b) {
            return a>b;
        });
        return rv;
        
    };`,
    out: {inline:1}
}
);