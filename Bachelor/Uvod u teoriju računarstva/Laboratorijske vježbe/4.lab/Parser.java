import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

public class Parser {
	static ArrayList<String> ulazniNiz=new ArrayList<>();
	static Integer count=0;
	static boolean jelNe=false;
	static StringBuilder sb=new StringBuilder();
	static String ulaz=new String();
	
	public static void parser() {
		
		ulaz=new String(ulazniNiz.get(count));
		S();
		
		if (!(ulaz.equals("\n"))) {
			jelNe=true;
		}
		
		if (jelNe)
			sb.append("\nNE");
		else
			sb.append("\nDA");
		System.out.println(sb);
		
		
	}
	
	public static void S() {
		
		sb.append("S");
		if (jelNe) {
			return;	
		}
		if (!(ulaz.equals("b"))  && !(ulaz.equals("a"))) {
			jelNe=true;
			return;
		}
		else if (ulaz.equals("a")) {								//aAB
			if (count<(ulazniNiz.size()-1)) {
				count++;
				ulaz=new String(ulazniNiz.get(count));
				A();
				
				if(jelNe) {
					return;
				}
				
				if (count< ulazniNiz.size()) {
					B();
					if (count.compareTo(ulazniNiz.size()-1) == 0) {
						
						return;
					}
				}
				
			}
			
		}
		else if (ulaz.equals("b")){									//bBA
			if (count<(ulazniNiz.size()-1)) {
				count++;
				ulaz=new String(ulazniNiz.get(count));
				if (count< ulazniNiz.size()) {
					B();
					if (count.compareTo(ulazniNiz.size()-1) == 0) {
						return;
					}
				}
				if(jelNe) {
					return;
				}
				if (count< ulazniNiz.size()) {
					A();
					if (count.compareTo(ulazniNiz.size()-1) == 0) {
						return;
					}
				}
				
			}
			
		}
		
	}
	
	public static void A() {
		
		sb.append("A");
		if (jelNe) {
			return;	
		}
		if (ulaz.equals("b")) {									
			if (count<(ulazniNiz.size()-1)) {
				count++;
				ulaz=new String(ulazniNiz.get(count));
				
				C();	
				if (count.compareTo(ulazniNiz.size()-1) == 0) {
					return;
				}
				if (jelNe) {
					return;
				}
				
			}
		}
		else if (ulaz.equals("c")) {
			jelNe=true;
		}
		else if (ulaz.equals("a")) {
			count++;
			ulaz=new String(ulazniNiz.get(count));
			
		}
		
	}
	
	public static void B() {
		
		sb.append("B");
		if (jelNe) {
			return;
		}
		if (ulaz.equals("c")) {														//c
			if (count<(ulazniNiz.size()-1)) {
				count++;
				ulaz=new String(ulazniNiz.get(count));
				if (ulaz.equals("c")) {												//c
					
					if (count<(ulazniNiz.size()-1)) {
						count++;
						ulaz=new String(ulazniNiz.get(count));
						
						if (count< ulazniNiz.size()) {								//S
							S();
							if (count.compareTo(ulazniNiz.size()-1) == 0) {
								return;
							}	
							if (jelNe) {
								return;
							}
							if (ulaz.equals("b")) {										//b
								if (count<(ulazniNiz.size()-1)) {
									count++;
									ulaz=new String(ulazniNiz.get(count));
									if (ulaz.equals("c")) {								//c
										if (count<(ulazniNiz.size()-1)) {
											count++;
											ulaz=new String(ulazniNiz.get(count));
											
										}
									}
									else {
										jelNe=true;
										return;
									}
								}
							}
							else {
								jelNe=true;
								return;
							}
							
						}
						
					}
					
				}
				else {
					jelNe=true;
					return;
				}
			}
			
		}
		
	}
	
	public static void C() {
		
		
		sb.append("C");
		if(jelNe) {
			return;
		}
		if (count< ulazniNiz.size()) {
			A();

			
			if (ulaz.equals("\n")) {
				jelNe=true;
				return;
			}
			if (jelNe) {
				return;
			}
			if (count< ulazniNiz.size()) {
				A();
			}
			
		}
		
		
	}
	
	public static void main(String[] args) throws IOException{
        BufferedReader br  = new BufferedReader(new InputStreamReader(System.in));		//citanje inputa
        //File file = new File("C:\\Users\\Korisnik\\Documents\\faks\\Uvod u teoriju racunarstva\\Emin lab\\primjer.txt");
        //BufferedReader br = new BufferedReader(new FileReader(file));
        String line=br.readLine();
        for (String znakic:line.split("")) {
        	ulazniNiz.add(znakic);
        }
        ulazniNiz.add("\n");
        br.close();
        parser();
		
	}
	
}
