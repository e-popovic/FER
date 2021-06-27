package os;

import java.util.Scanner;
import java.util.Random;

public class Simulacija {


	public static void main(String[] args) {
		Scanner sc=new Scanner(System.in);
		Integer velicinaSpremnika=0,i;
		String id="0";
		
		if (args.length == 1) {										//velicina se upisuje ili iz cmd
			velicinaSpremnika=Integer.parseInt(args[0]);				
		}
		else {
			System.out.printf("Velicina spremnika? ");				//ili se skenira
			velicinaSpremnika=sc.nextInt();
		}
		
		String[] spremnik= new String[velicinaSpremnika];
		for(i=0;i<velicinaSpremnika;i++) {
			spremnik[i]= "-";
		}
		printajSpremnik(spremnik,velicinaSpremnika);				//spremnik napravljen
		
		while(sc.hasNext()) {
			String line=sc.nextLine();
			
			if (line.toUpperCase().equals("KRAJ")) {	//izlaz
				break;
			}
			
			else if ( line.toUpperCase().equals("O")) {		//oslobadanje
				System.out.printf("\nKoji zahtjev treba osloboditi? ");
				String line2=sc.nextLine();
				oslobodiSpremnik(spremnik,line2,velicinaSpremnika);
				
			}
			
			else if (line.toUpperCase().equals("Z")) {	//zahtjev
				System.out.printf("\nKoliko mjesta zauzima zahtjev %s? ",id);
				Integer velicinaZahtjeva=sc.nextInt();
				
				//Integer velicinaZahtjeva = (int)(Math.random() * velicinaSpremnika + 1);
				//System.out.printf("\nZahtjev %s1 zauzima %d spremnickih lokacija \n",id,velicinaZahtjeva);
				
				if ((nadiMjesto(spremnik,velicinaSpremnika,velicinaZahtjeva,id))==0) 
					System.out.printf("\nNije izvedeno, nema dovoljno praznog prostora!\n");
					
				
				
				printajSpremnik(spremnik,velicinaSpremnika);
				
				Integer bid=Integer.parseInt(id) + 1;
				id=String.valueOf(bid);
				
			}
			
			else			//nije zahtjev ni oslobadanje ni kraj? nastavi								
				continue;	
		}
		
		sc.close();
	}
	
	public static void printajSpremnik(String[] spremnik,Integer velicina) {
		for(Integer i=0;i<velicina;i++) {
			System.out.printf("%s",spremnik[i]);
		}
		System.out.printf("\n");
	}
	public static void oslobodiSpremnik(String[] spremnik,String wantedLine,Integer velicina) {
		for(Integer i=0;i<velicina;i++) {
			if(spremnik[i].equals(wantedLine))
				spremnik[i]="-";
		}
		printajSpremnik(spremnik,velicina);
		
	}
	
	public static Integer nadiMjesto(String[] spremnik,Integer velicinaSpremnika,Integer velicinaZahtjeva,String id) {
		Integer i,j,trenutnoPrazno=0,pozicijaTrenPrazno=0,prvi=0,minPrazno=0,pozicijaMinPrazno=0;		
		
		Integer ukupnoRupe=0;							//ako je prevelik zahtjev odmah izaði 
		for(String clan:spremnik) {
			if (clan.equals("-"))
				ukupnoRupe++;
		}
		if (ukupnoRupe<velicinaZahtjeva)
			return 0;
		
		for (i=0;i<velicinaSpremnika;i++) {
			trenutnoPrazno=0;
			pozicijaTrenPrazno=0;
			if (spremnik[i].equals("-")) {
				pozicijaTrenPrazno=i;
				while(i<velicinaSpremnika && spremnik[i].equals("-")) {
					trenutnoPrazno++;
					i++;
				}
				if (trenutnoPrazno>=velicinaZahtjeva) {
					if (prvi==0) {
						prvi=1;
						pozicijaMinPrazno=pozicijaTrenPrazno;
						minPrazno=trenutnoPrazno;
					}
					else if (trenutnoPrazno<minPrazno){
						pozicijaMinPrazno=pozicijaTrenPrazno;
						minPrazno=trenutnoPrazno;
					}
				}
			}
		}
		
		if (minPrazno<velicinaZahtjeva) {
			defragmentacija(spremnik,velicinaSpremnika);
			for (j=0;j<velicinaSpremnika;j++) {
				if (spremnik[j].equals("-")) 
					if(velicinaSpremnika-j < velicinaZahtjeva)
						return 0;	//nema dovoljno praznog prostora
					else {
						pozicijaMinPrazno=j;		
						break;
					}
			}
			
		}
		for(j = pozicijaMinPrazno; j < pozicijaMinPrazno + velicinaZahtjeva; j++) {
			if (j>=velicinaSpremnika) break;
			spremnik[j] = id; 
		}
			
		return 1;
		
	}
	
	
	
	public static void defragmentacija(String[] spremnik,Integer velicinaSpremnika) {
		Integer j,i;
		String pomoc;
		for (i=0;i<velicinaSpremnika;i++) {			//bubble sort stavlja sve rupe na kraj
			for (j=1;j<(velicinaSpremnika-1);j++) {
				if (spremnik[j-1].equals("-")) {
					pomoc=spremnik[j-1];
					spremnik[j-1]=spremnik[j];
					spremnik[j]=pomoc;
				}
			}	
		}
		System.out.printf("\nDolazi do defragmentacije\n");
		printajSpremnik(spremnik,velicinaSpremnika);
	}
}
