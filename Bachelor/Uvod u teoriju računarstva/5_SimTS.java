//package pet;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;

public class SimTS {
	static HashMap<String,String> map=new HashMap<>(); // trenutnoStanje, znakNaTraci -> novoStanje, noviZnakNaTraci, PomakGlave
	static HashSet<String> stanja=new HashSet<>();
	static HashSet<String> ulazniZnakovi=new HashSet<>();
	static HashSet<String> znakoviTrake=new HashSet<>();
	static String praznaCelija;
	static String[] traka;
	static HashSet<String> prihvatljivaStanja=new HashSet<>();
	static String pocetnoStanje;
	static String trenutnoStanje;
	static Integer pocetnaTraka;
	static Integer trenutnaTraka;
	static StringBuilder sb=new StringBuilder();
	
	public static void TuringovStroj() {
		trenutnoStanje=pocetnoStanje;
		trenutnaTraka=pocetnaTraka;
		
		while(true) {
			String kljuc=trenutnoStanje+","+traka[trenutnaTraka];
			String vrijednost=map.get(kljuc);
			
			if (vrijednost == null)		//nema prijelaza? vrati
				return;
			
			else {
				String novoStanje=vrijednost.split(",")[0];
				String noviZnakNaTraci=vrijednost.split(",")[1];
				String pomakGlave=vrijednost.split(",")[2];
				
				if (trenutnaTraka.compareTo(0) == 0 && pomakGlave.equals("L")) {
					return;
				}
				else if (trenutnaTraka.compareTo(traka.length-1) == 0 && pomakGlave.equals("R")) {
					return;
				}
				
				traka[trenutnaTraka]=noviZnakNaTraci;
				trenutnoStanje=novoStanje;
				if (pomakGlave.equals("L"))
					trenutnaTraka--;
				else
					trenutnaTraka++;			
			}
		}
		
	}
	
	public static void printaj() {
		sb.append(trenutnoStanje+"|"+trenutnaTraka+"|");
		for (String element:traka) {
			sb.append(element);
		}
		if (prihvatljivaStanja.contains(trenutnoStanje))
			sb.append("|1");
		else
			sb.append("|0");
		System.out.println(sb);
	}
	
	public static void main(String[] args) throws IOException{
		int i = 0;
        String line;
        BufferedReader br  = new BufferedReader(new InputStreamReader(System.in));		//citanje inputa
        //File file = new File("C:\\Users\\Korisnik\\Documents\\faks\\Uvod u teoriju racunarstva\\Emin lab\\primjer.txt");
        //BufferedReader br = new BufferedReader(new FileReader(file));
        while ((line = br.readLine()) != null) {
        	switch(i) {
        		case 0:
        			stanja.addAll(Arrays.asList(line.split(",")));
                    break;
        		case 1:
        			ulazniZnakovi.addAll(Arrays.asList(line.split(",")));
                    break;
        		case 2:
        			znakoviTrake.addAll(Arrays.asList(line.split(",")));
                    break;
        		case 3:
        			praznaCelija = line;
        			break;
        		case 4:
        			traka=line.split("");
                    break;
        		case 5:
        			prihvatljivaStanja.addAll(Arrays.asList(line.split(",")));
                    break;
        		case 6:
        			pocetnoStanje = line;
        			break;
        		case 7:
        			pocetnaTraka = Integer.parseInt(line);
        			break;
        		default:
        			String[] prvi = line.split("->");        // trenutnoStanje, znakNaTraci -> novoStanje, noviZnakNaTraci, PomakGlave
        			map.put(prvi[0],prvi[1]);
        			break;
        	}
        	i++;
        }
        br.close();
        
        TuringovStroj();
        
        printaj();

	}

}
