import java.io.*;
import java.util.*;

public class SimEnka {
    static String[] niz = null, skupStanja = null, skupSimbola = null, prihStanja = null;
    static String pZnak = null;
    static HashMap<String[], String> mapa = new HashMap<>();        //mapa: KEY= STRING[0] OTKUD--STRING[1] DOKUD,, VALUE=  ONO NA STRELICI
    static Set<String> trenStanje = new TreeSet<String>();

    public static Set<String> getKeysByValue(Map<String[], String> map, String value, String startna) {
        Set<String> keys = new HashSet<String>();
        for (Map.Entry<String[], String> entry : map.entrySet()) {                //imamo pocetno stanje i strelicu,trazimo zavrsno
            String[] key = entry.getKey();
            if (Objects.equals(value, entry.getValue()) //|| Objects.equals("$", entry.getValue()))
                    && Objects.equals(startna, key[0])) {
                keys.add(key[1]);
                break;
            }
        }
        return keys;
    }

    public static Set<String> jelEpsilon(Map<String[], String> map, Set<String> sviprelazi) {
        Set<String> keys = new HashSet<String>();

        for (String prelaz : sviprelazi) {                                        //dal ima ikoje zavrsno stanje (prelaz[1]) jos epsilon
            for (String dioprelaza : prelaz.split(",")) {        //sva zavrsna stanja

                for (Map.Entry<String[], String> entry : map.entrySet()) {
                    String[] key = entry.getKey();
                    if (Objects.equals("$", entry.getValue()) && Objects.equals(key[0], dioprelaza))
                        keys.add(key[1]);
                }
            }

        }
        return keys;
    }

    public static void main(String[] args) throws IOException {
        int i = 0;
        String line;
        BufferedReader br  = new BufferedReader(new InputStreamReader(System.in));		//citanje inputa
        File file = new File("test.a");
        //BufferedReader br = new BufferedReader(new FileReader(file));
        while ((line = br.readLine()) != null) {

            switch (i) {
                case 0:
                    niz = line.split("\\|");
                    break;
                case 1:
                    skupStanja = line.split(",");
                    break;
                case 2:
                    skupSimbola = line.split(",");
                    break;
                case 3:
                    prihStanja = line.split(",");
                    break;
                case 4:
                    pZnak = line;

                    break;
                default:
                    String[] prvi = line.split("->");        // otkud,strelica -> kamo
                    String[] lijevo = prvi[0].split(",");  //DESNO MOZE BITI VISE VRIJEDNOSTI!!!!!
                    if (lijevo[1] == "#")
                        break;
                    String[] odgdje = new String[]{lijevo[0], prvi[1]};
                    mapa.put(odgdje, lijevo[1]);

                    break;

            }
            i++;

        }
        br.close();
		
		/*System.out.printf("\nmapa\n");
		for (String[] key: mapa.keySet()) {
		    System.out.println("key : " + Arrays.toString(key));
		    System.out.println("value : " + (mapa.get(key)));
		}*/
        StringBuilder sb = new StringBuilder();
        for (i = 0; i < niz.length; i++) {
            List<String> simboli = Arrays.asList(niz[i].split(","));        //simbol= u mapi values

            Set<String> keys = new TreeSet<String>();
            trenStanje.add(pZnak);
            keys.add(pZnak);
//            sb.append(pZnak);
//            for (Map.Entry<String[], String> entry : mapa.entrySet()) {
//                String[] key = entry.getKey();
//                if (Objects.equals("$", entry.getValue()) && Objects.equals(key[0], pZnak))
//                    keys.addAll(Arrays.asList(key[1].split(",")));
//            }

            while (!(keys.isEmpty())) {
                //System.out.printf("\nwhile");
//                trenStanje.addAll(keys);
                Set<String> hkeys = new TreeSet<String>(keys);
                Set<String> novi = new TreeSet<String>();
                for (String h : hkeys) {
                    for (Map.Entry<String[], String> entry : mapa.entrySet()) {
                        String[] key = entry.getKey();
                        if (Objects.equals("$", entry.getValue()) && Objects.equals(key[0], h))
                            novi.addAll(Arrays.asList(key[1].split(",")));//
                        
                    }
                }
                if(keys.containsAll(novi))
                	break;
                
                keys.addAll(novi);
                novi.clear();
            }
            if (!(keys.isEmpty())) {
//                sb.append(",");
                for (String k : keys) {
                    sb.append(k + ",");
                }
                sb.setLength(sb.length() - 1);
                sb.append("|");
                trenStanje.addAll(keys);

            }
//            sb.append("|");

            for (String simbol : simboli) {
                int nemadalje = 0, kolkoihima = trenStanje.size();
                Set<String> buducizn = new TreeSet<>();

                for (String tstanje : trenStanje) {            //tstanje= u mapi key [0]
                    Set<String> sviprelazi = new HashSet<>();
                    sviprelazi = getKeysByValue(mapa, simbol, tstanje);        //sviprelazi=bez koristenje epsilon


                    if (!(sviprelazi.isEmpty()) && !(sviprelazi.size()==1 && sviprelazi.contains("#"))) {

                        //List<String> buducizn = new ArrayList<>();
                        for (String prel : sviprelazi) {                    //MOZE BIT PRELAZ U VISE STANJA!!!!!!!!!
                            buducizn.addAll(Arrays.asList(prel.split(",")));
                        }

                        Set<String> epsprelazi = new HashSet<>();
                        epsprelazi = jelEpsilon(mapa, sviprelazi);

                        Set<String> novi = new TreeSet<>();
                        while (!(epsprelazi.isEmpty())) {
                            for (String e : epsprelazi) {
                                String[] hjoj = e.split(",");
                                for (String h : hjoj) {
                                    novi.add(h);
                                }   
                            }
                            if(buducizn.containsAll(novi))
                            	break;
                            
                            buducizn.addAll(novi);
                            novi.clear();
                            epsprelazi = jelEpsilon(mapa, epsprelazi);
                        } //popunimo buducizn sa svim iducim mogucim stanjima
                    } else {
                        nemadalje++;
                    }
                }

                if (!buducizn.isEmpty()) {
                   // Collections.sort(buducizn);
                	if(buducizn.contains("#"))
                		buducizn.remove("#");
                	
                    for (String s : buducizn) {
                        sb.append(s + ",");
                    }
                    sb.setLength(sb.length() - 1);
                    sb.append("|");

                    trenStanje = buducizn;
                }

//					else {
//						nemadalje++;
//
//
//					}


                if (nemadalje == kolkoihima) {
                    sb.append("#|");
                    nemadalje = 0;
                    trenStanje.clear();
                }

                //System.out.printf("Proso jedan simbol\n");
            }
            sb.setLength(sb.length() - 1);        //makni zadnji |
            sb.append("\n");

        }
        sb.setLength(sb.length() - 1);
        System.out.println(sb);
    }


}
