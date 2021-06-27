//package lab3;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.Objects;
import java.util.TreeSet;

public class SimPa {
	static LinkedHashMap<String,String> map= new LinkedHashMap<>(); 	//[trenutnoStanje,ulazniZnak,znakStoga] [novoStanje,nizZnakovaStoga]	
	static LinkedList<String> ulazniNizovi= new LinkedList<>();
	static TreeSet<String> stanja=new TreeSet<>();
	static TreeSet<String> ulazniZnakovi=new TreeSet<>();
	static TreeSet<String> znakoviStoga=new TreeSet<>();
	static TreeSet<String> prihvatljivaStanja=new TreeSet<>();
	static String pocetnoStanje=new String();
	static String pocetniZnakStoga=new String();
	
	public static void potisniAutomat() {
		
		//stvori stog, trenutnoStanje, stringbuilder
		String trenutnoStanje=new String(pocetnoStanje);
		ArrayList<String> trenutniStog=new ArrayList<>();
		StringBuilder sb=new StringBuilder();
		boolean jelEpsilon=true;
		
		//prolaz kroz sve ulazne nizove
		for (String stringUlazniNiz : ulazniNizovi) {
			trenutnoStanje=new String(pocetnoStanje);
			trenutniStog.clear();
			trenutniStog.add(0,pocetniZnakStoga);
			sb.append(trenutnoStanje + "#" + stogUString(trenutniStog) + "|");
			String[] ulazniNiz=stringUlazniNiz.split(",");
			boolean fail=false;
			
			//prolazi kroz sve znakove jednog ulaznog niza
			for (String ulazniZnak : ulazniNiz) {
				String kljuc=new String();
				
				//WHILE PETLJU AK JE EPSILON PRIJELAZ BIO
				jelEpsilon=true;
				
				while(jelEpsilon) {
					jelEpsilon=false;
					if (trenutniStog.isEmpty()) {
						kljuc= trenutnoStanje + "," + ulazniZnak + "," + "$";
					}
					else {
						kljuc= trenutnoStanje + "," + ulazniZnak + "," + trenutniStog.get(0);
					}
					if (map.containsKey(kljuc)) {
						String vrijednost = map.get(kljuc);
						String novoStanje=vrijednost.split(",")[0];
						String nizZnakovaStoga=vrijednost.split(",")[1];
						
						trenutnoStanje=new String(novoStanje);
						
						if (Objects.equals("$", nizZnakovaStoga)) {	//ako se mice znak sa stoga
							trenutniStog.remove(0);
						}
						
						else if(nizZnakovaStoga.length()==1 && trenutniStog.isEmpty()) {	//ako se na prazni stog stavlja nesto
							trenutniStog.add(0,nizZnakovaStoga);
						}
						else {
							if (!(trenutniStog.isEmpty())) {
								trenutniStog.remove(0);
							}
							String[] znakovi=nizZnakovaStoga.split("");
							for (int i=znakovi.length-1;i>=0;i--) {
								trenutniStog.add(0, znakovi[i]);
							}
						}
						
						sb.append(trenutnoStanje + "#" + stogUString(trenutniStog) + "|");
					}
					else {			//ako nema tog prijelaza dal ima bar epsilon prijelaz?
						String kljucEpsilon=new String();
						if (trenutniStog.isEmpty()) {
							kljucEpsilon= trenutnoStanje + "," + "$" + "," + "$";
						}
						else {
							kljucEpsilon= trenutnoStanje + "," + "$" + "," + trenutniStog.get(0);
						}
						
						
						if (map.containsKey(kljucEpsilon)) {
							jelEpsilon=true;
							String vrijednost = map.get(kljucEpsilon);
							String novoStanje=vrijednost.split(",")[0];
							String nizZnakovaStoga=vrijednost.split(",")[1];
							
							trenutnoStanje=new String(novoStanje);
							
							if (Objects.equals("$", nizZnakovaStoga)) {	//ako se mice znak sa stoga
								trenutniStog.remove(0);
							}
							
							else if(nizZnakovaStoga.length()==1 && trenutniStog.isEmpty()) {	//ako se na prazni stog stavlja nesto
								trenutniStog.add(0,nizZnakovaStoga);
							}
							else {
								if (!(trenutniStog.isEmpty())) {
									trenutniStog.remove(0);
								}
								String[] znakovi=nizZnakovaStoga.split("");
								for (int i=znakovi.length-1;i>=0;i--) {
									trenutniStog.add(0, znakovi[i]);
								}
							}
							
							sb.append(trenutnoStanje + "#" + stogUString(trenutniStog) + "|");
						}
						
						else {
							sb.append("fail|");
							fail=true;
							break;
						}
					}
				}
				if (fail)
					break;
				
			}
			
			//dal nakon kraja niza postoji jos jedan epsilon prijelaz
			boolean krajEpsilonPrijelaz=true;
			if (!fail  && !(prihvatljivaStanja.contains(trenutnoStanje))) {	
				while(krajEpsilonPrijelaz && !(prihvatljivaStanja.contains(trenutnoStanje))) {
					krajEpsilonPrijelaz=false;
					
					String kljucEpsilon=new String();								
					if (trenutniStog.isEmpty()) {
						kljucEpsilon= trenutnoStanje + "," + "$" + "," + "$";
					}
					else {
						kljucEpsilon= trenutnoStanje + "," + "$" + "," + trenutniStog.get(0);
					}
					
					
					if (map.containsKey(kljucEpsilon)) {
						krajEpsilonPrijelaz=true;
						String vrijednost = map.get(kljucEpsilon);
						String novoStanje=vrijednost.split(",")[0];
						String nizZnakovaStoga=vrijednost.split(",")[1];
						
						trenutnoStanje=new String(novoStanje);
						
						if (Objects.equals("$", nizZnakovaStoga)) {	//ako se mice znak sa stoga
							trenutniStog.remove(0);
						}
						
						else if(nizZnakovaStoga.length()==1 && trenutniStog.isEmpty()) {	//ako se na prazni stog stavlja nesto
							trenutniStog.add(0,nizZnakovaStoga);
						}
						else {
							if (!(trenutniStog.isEmpty())) {
								trenutniStog.remove(0);
							}
							String[] znakovi=nizZnakovaStoga.split("");
							for (int i=znakovi.length-1;i>=0;i--) {
								trenutniStog.add(0, znakovi[i]);
							}
						}
						
						sb.append(trenutnoStanje + "#" + stogUString(trenutniStog) + "|");
					}
				}
				
			}
			
			//jel niz prihvatljiv
			if (prihvatljivaStanja.contains(trenutnoStanje) && !fail) {
				sb.append("1"+"\n");
			}
			else {
				sb.append("0"+"\n");
			}
			
		}
		sb.setLength(sb.length() - 1);
        System.out.println(sb);
		
	}
	
	public static String stogUString(ArrayList<String> stog) {	
		String string=new String();
		if (stog.isEmpty()) {
			string="$";
		}
		else {
			for (String elementStoga:stog) {
				string+=elementStoga;
			}
		}
		return string;
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
        			ulazniNizovi.addAll(Arrays.asList(line.split("\\|")));
                    break;
        		case 1:
        			stanja.addAll(Arrays.asList(line.split(",")));
                    break;
        		case 2:
        			ulazniZnakovi.addAll(Arrays.asList(line.split(",")));
                    break;
        		case 3:
        			znakoviStoga.addAll(Arrays.asList(line.split(",")));
                    break;
        		case 4:
        			prihvatljivaStanja.addAll(Arrays.asList(line.split(",")));
                    break;
        		case 5:
        			pocetnoStanje = line;
        			break;
        		case 6:
        			pocetniZnakStoga = line;
        			break;
        		default:
        			String[] prvi = line.split("->");        
        			map.put(prvi[0],prvi[1]);
        			break;
        	}
        	i++;
        }
        br.close();
        
        potisniAutomat();

	}
	
}
