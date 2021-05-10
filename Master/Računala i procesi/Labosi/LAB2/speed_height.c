#include <p18f1220.h>
#pragma config WDT = OFF
#pragma config PWRT= ON
#pragma config MCLRE=OFF // zadano

unsigned int racunbrzine = 0; // provjera racuna li se brzina (ili visina)
unsigned int motori = 0; //  brojac motora
unsigned int osobna = 0; // brojac osobnih vozila
unsigned int teretna = 0; // brojac teretnih vozila
unsigned int ostala = 0; // brojac ostalih vozila

void high_isr(void); // prototip prekida

#pragma code high_vector=0x08 // prekidni vektor

void interrupt_at_high_vector(void){
     _asm GOTO high_isr _endasm // prekidna rutina 
}
#pragma code // gotov prekidni vektor

#pragma interrupt high_isr 
// prekidna rutina visokog prioriteta
void high_isr (void){
   
   if (INTCON3bits.INT2IF) {	// prekid zbog stisnutog gumba
      
      ADCON0bits.GO = 1;	// pocni A/D konverziju brzine
      racunbrzine = 1; // racuna se brzina od sad
      INTCON3bits.INT2IF= 0; // reset zastavice
   }    
   
   else if (PIR1bits.ADIF) { // zavrsena A/D konverzija
      
      if (racunbrzine) {	// zavrsilo je racunanje brzine
	 if ( ( ADRESH > 0 ) || ( ADRESL >0xB2 ) ) { // brzina veca od 60km/h
	    LATB = 0x10; // upali LED
	 }
	 else { // brzina manja ili jednaka 60km/h
	    LATB = 0x00; // zgasi LED
	 }
	 
	 racunbrzine = 0; // vise se ne racuna brzina
	 
	 ADCON0 = 0x05; // A/D, analog input AN1, Fosc/2 za A/D (iduca ide visina) 
	 ADCON0bits.GO = 1; // pocni A/D konverziju visine
      }
      
      else {	// zavrsilo je racunanje visine
	 if ( ADRESH >= 0x02 ) { // teretno vozilo, preko 2m
	    ++teretna; // povecaj potrebni brojac
	 }
	 else if ( ( ADRESH == 0x01 ) ) { //  1m - 2m
	    if ( ADRESL >= 0x66 ) { // osobno vozilo, 1.4m - 2m
	       ++osobna; // povecaj potrebni brojac
	    }
	    else { // motor, 1m - 1.4m
	       ++motori; // povecaj potrebni brojac 
	    }
	 }
	 else { // ostalo, ispod 1m
	    ++ostala; // povecaj potrebni brojac 
	 }

	 ADCON0 = 0x01; // A/D, analog input AN0, Fosc/2 za A/D (iduca ide brzina)
      }
      
      PIR1bits.ADIF = 0; // reset zastavice
   }
   
}
   
#pragma code // gotov prekidni vektor


void main(void){   
   PORTB = 0; // ocisti port B
   PORTA = 0; // ocisti port A
   
   TRISB = 0x04; //  maska – ulaz RB2/INT2
   TRISA = 0x03; // maska – ulazi A/D za visinu (AN1) i brzinu (AN0)
   
   ADCON0 = 0x01; // A/D, analog input AN0, Fosc/2 za A/D
   ADCON1 = 0xFC; // AN0 i AN1 A/D	
   ADCON2 = 0x8D; // Fosc/16, 8 TAD, desno poravnano
   
   PIE1bits.ADIE = 1; // omogući generiranje prekida od A/D jedinice
   
   INTCON3 = 0x10; // dozvoli INT2 prekide
   INTCON = 0xC0; // dozvoli globalne prekide i periferne prekide
   
   while(1){} // beskonacna petlja
}
