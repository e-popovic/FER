import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Objects;
import java.util.TreeMap;
import java.util.TreeSet;

public class MinDka {

	static LinkedHashMap<String,String> map= new LinkedHashMap<>();		//mapa:   key = String[] = trenutnoStanje, simbolAbecede
	static TreeSet<String> stanja=new TreeSet<>();									//        value = TreeSet<String> = iducaStanja 
	static TreeSet<String> simboliAbecede=new TreeSet<>();
	static TreeSet<String> prihvatljivaStanja=new TreeSet<>();
	static String pocetnoStanje;
	
	public static void ukloniNedohvatljivaStanja() {
		
		TreeSet<String> dohvatljivaStanja=new TreeSet<>();
		TreeSet<String> novaDohvatljivaStanja=new TreeSet<>();
		dohvatljivaStanja.add(pocetnoStanje);
		int prije=0,poslije=1;
		
		while(prije!=poslije) {
			poslije=0;
			prije=dohvatljivaStanja.size();
			for (String stanje: dohvatljivaStanja) {
				for (Map.Entry<String,String> entry : map.entrySet()) {                //imamo pocetno stanje i strelicu,trazimo zavrsno
		            String kljuc = entry.getKey();
		            String[] key=kljuc.split(",");
		            if (Objects.equals(key[0],stanje)) {
		                	novaDohvatljivaStanja.add(entry.getValue()); 
		            }
		        }
			}
			
			dohvatljivaStanja.addAll(novaDohvatljivaStanja);
			novaDohvatljivaStanja.clear();
			poslije=dohvatljivaStanja.size();
		}
		
		//mjenjaj map, stanja, prihvatljivaStanja
		
		//System.out.println(Arrays.toString(dohvatljivaStanja.toArray()));
		
		TreeSet<String> nedohvatljivaStanja=new TreeSet<>();
		for(String stanje: stanja) {
			if (!(dohvatljivaStanja.contains(stanje)))
				nedohvatljivaStanja.add(stanje);
		}
		//System.out.println(Arrays.toString(nedohvatljivaStanja.toArray()));
		stanja=new TreeSet<>(dohvatljivaStanja);
		
		LinkedHashMap<String,String> noviMap=new LinkedHashMap<>(map); 
		for(String s:nedohvatljivaStanja) {
				for (Map.Entry<String,String> entry : map.entrySet()) {                //imamo pocetno stanje i strelicu,trazimo zavrsno
					String kljuc = entry.getKey();
		            String[] key=kljuc.split(",");
		            if (Objects.equals(key[0],s) ) {
		            	noviMap.remove(kljuc);
		            	
		            	
		            }
		        }
		}
		map=new LinkedHashMap<>(noviMap);
		
		TreeSet<String> novaPrihvatljivaStanja=new TreeSet<>();
		for(String s:prihvatljivaStanja) {
			if (dohvatljivaStanja.contains(s))
				novaPrihvatljivaStanja.add(s);
		}
		prihvatljivaStanja=new TreeSet<>(novaPrihvatljivaStanja);
	}
	
	public static void minimizacija() {
		//2.algoritam --> Jezicni procesori pdf str27
		
		//1.KORAK
		TreeSet<String> neprihvatljivaStanja=new TreeSet<>();
		for (String stanje:stanja) {
			if(!(prihvatljivaStanja.contains(stanje)))
				neprihvatljivaStanja.add(stanje);
		}
		TreeMap<String,TreeSet<String>> pi = new TreeMap<>(); //pi=prihvatljiva, pa neprihvatljiva stanja
		TreeMap<String,TreeSet<String>> noviPi = new TreeMap<>();
		pi.put("1",prihvatljivaStanja);
		pi.put("0",neprihvatljivaStanja);
		
		
		//2.KORAK==algoritam
		boolean prvi=true;
		Integer velicinaPi=pi.size();
		Integer velicinaNovogPi=0;
		
		while(velicinaPi != velicinaNovogPi){
			if (!prvi) {
				pi=new TreeMap<>(noviPi);
				noviPi.clear();
			}
			if (prvi)
				prvi=false;
		
			for (Map.Entry<String,TreeSet<String>> skupinaStanja : pi.entrySet()) {	//skup stanja sadrzi sva prihvatljiva ILI neprihvatljiva stanja
				TreeSet<String> skupStanja=skupinaStanja.getValue();
				
				for (String stanje:skupStanja) {
					String diIde=skupinaStanja.getKey();
					for (String simbol:simboliAbecede) {				
						String kljuc= stanje+","+simbol;
						String value= map.get(kljuc);
						for (Map.Entry<String,TreeSet<String>> entry : pi.entrySet()) {
							if (entry.getValue().contains(value))
								diIde+=entry.getKey();
						}
					}
					if (noviPi.get(diIde) == null) {
						TreeSet<String> set=new TreeSet<String>();
						noviPi.put(diIde,set);
					}
					TreeSet<String> value= noviPi.get(diIde);
					value.add(stanje);
					noviPi.put(diIde,value);
				}
			}
			velicinaPi=pi.size();
			velicinaNovogPi=noviPi.size();
		}
		
		/*for (String key: pi.keySet()) {
		    System.out.println("key : " + key);
		    System.out.println("value : " + (pi.get(key)));
		}*/
		
		//3.KORAK
		//pi u sebi ima setove,prvi clan svakog seta ostaje,ostali clanovi seta se zamjenjuju prvim
		//mjenjaj map, stanja, prihvatljivaStanja, pocetnoStanje
		
		for (Map.Entry<String,TreeSet<String>> skupinaStanja : pi.entrySet()) {
			String[] skupStanja = skupinaStanja.getValue().toArray(new String[skupinaStanja.getValue().size()]);
			String ostavljam=skupStanja[0];
			
			if (skupStanja.length>1) {
				for (Integer i=1;i<skupStanja.length;i++) {
					String micem=new String(skupStanja[i]);
					
					//pocetnoStanje
					if (micem.compareTo(pocetnoStanje) == 0)
						pocetnoStanje=ostavljam;
					
					//prihvatljivaStanja
					if(prihvatljivaStanja.contains(micem))
						prihvatljivaStanja.remove(micem);
					
					//stanja
					stanja.remove(micem);
					
					//map
					LinkedHashMap<String,String> noviMap= new LinkedHashMap<>();
					for (Map.Entry<String,String> entry : map.entrySet()) {
						String kljuc = entry.getKey();
			            String[] key=kljuc.split(",");
			            String value = map.get(kljuc);
			            
			            if(!(micem.equals(key[0]))) {
			            	
			            	if(micem.equals(value)) {
				            	value=ostavljam;
				            }
			            	noviMap.put(kljuc,value);
			            }
			            
					}
					map = new LinkedHashMap<>(noviMap);
				}
			}
		}
	}
	
	public static void printaj() {
		StringBuilder sb=new StringBuilder();
        for(String stanje:stanja) {
        	sb.append(stanje+",");
        }
        sb.setLength(sb.length() - 1);        //makni zadnji ,
        sb.append("\n");
        
        for(String simbol:simboliAbecede) {
        	sb.append(simbol+",");
        }
        sb.setLength(sb.length() - 1);
        sb.append("\n");
        
        for(String stanje:prihvatljivaStanja) {
        	sb.append(stanje+",");
        }
        if (!(prihvatljivaStanja.isEmpty())) {
        	sb.setLength(sb.length() - 1);
        }
        sb.append("\n");
        
        sb.append(pocetnoStanje);
        sb.append("\n");
        
        for (Map.Entry<String,String> entry : map.entrySet()) {                //imamo pocetno stanje i strelicu,trazimo zavrsno
        	String kljuc = entry.getKey();
            String[] key=kljuc.split(",");
            String value=entry.getValue();
            sb.append(key[0]+","+key[1]+"->"+value);
            sb.append("\n");
        }
        sb.setLength(sb.length() - 1);
        System.out.println(sb);
        
	}
	
	public static void main(String[] args) throws IOException{
		
		int i = 0;
        String line;
        BufferedReader br  = new BufferedReader(new InputStreamReader(System.in));		//citanje inputa
        File file = new File("C:\\Users\\Korisnik\\Documents\\faks\\Uvod u teoriju racunarstva\\Emin lab\\primjer.txt");
        //BufferedReader br = new BufferedReader(new FileReader(file));
        while ((line = br.readLine()) != null) {
        	switch(i) {
        		case 0:
        			stanja.addAll(Arrays.asList(line.split(",")));
                    break;
        		case 1:
        			simboliAbecede.addAll(Arrays.asList(line.split(",")));
                    break;
        		case 2:
        			prihvatljivaStanja.addAll(Arrays.asList(line.split(",")));
                    break;
        		case 3:
        			pocetnoStanje = line;
        			break;
        		default:
        			String[] prvi = line.split("->");        // otkud,strelica -> kamo
        			map.put(prvi[0],prvi[1]);
        			break;
        	}
        	i++;
        }
        br.close();
        
        ukloniNedohvatljivaStanja();
        
        minimizacija();
        
        printaj();
        
        //miro je bio ovdje <3 omg
        //2.5.2019
        //vrati se.
        
	}
	
}
