#include <p18f1220.h>
#pragma config WDT = OFF
#pragma config PWRT= ON
#pragma config MCLRE=OFF // zadano

unsigned int state = 1; // globalna varijabla stanje, pocetno - crveno
unsigned int stojiSemaforB = 2; // brojac semafora B - prva dva prekida se ne mice s crvenog
void high_isr(void); // prototip prekida

#pragma code high_vector=0x08 // prekidni vektor

void interrupt_at_high_vector(void){
     _asm GOTO high_isr _endasm // prekidna rutina 
}
#pragma code // gotov prekidni vektor

#pragma interrupt high_isr 
// prekidna rutina visokog prioriteta
void high_isr (void){
   INTCON = 0xA0; // dozvoli globalne prekide i prekid timera 0
   T0CON = 0x88; //  pokreni timer0, 16-bitni, interni oscilator, ne koristi prescaler
   
   if ((state == 0) || (state ==2)){	// crveno/zeleno --> 3 sekunde
      TMR0H = 0xE8; // oko 59536, gornji bitovi
      TMR0L = 0xAE; //  donji bitovi	B0
   }
   else {	// prijelazna stanja --> 2 sekunde
      TMR0H = 0xF0; // oko 61536, gornji bitovi
      TMR0L = 0x7B; //  donji bitovi	7D
   }
   
   if (stojiSemaforB) {	// B stoji u crvenom, A se mice, upali tocna svijetla
      switch (state){
	 case 0:
	    PORTB = 0x11;	// crveno crveno
	    break;
	 case 1:
	    PORTB = 0x31;	// crveno+zuto crveno
	    break;
	 case 2:
	    PORTB = 0x41;	// zeleno crveno
	    break;
	 case 3:
	    PORTB = 0x21;	// zuto crveno
	    break;
      }   
      --stojiSemaforB; // umanji brojac
   }
   else {	// oba semafora se micu, upali tocna svijetla
      switch (state){
	 case 0:
	    PORTB = 0x14;	// crveno zeleno
	    break;
	 case 1:
	    PORTB = 0x32;	// crveno+zuto zuto
	    break;
	 case 2:
	    PORTB = 0x41;	// zeleno crveno
	    break;
	 case 3:
	    PORTB = 0x23;	// zuto crveno+zuto
	    break;
      } 
   } 
   
   state = (state + 1) % 4; // promijeni stanje
}
   
#pragma code // gotov prekidni vektor


void main(void){
   PORTB = 0x11; // semafori pocinju u crvenom
   TRISB = 0; //sve linije izlazne

   INTCON = 0xA0; // dozvoli globalne prekide i prekid timera 0
   T0CON = 0x88; //  pokreni timer0, 16-bitni, interni oscilator, ne koristi prescaler
   TMR0H = 0xE9; // postavljamo trajanje od 3 sekundi, oko 65536 – 6000 = 65536, gornji bitovi
   TMR0L = 0x29; //  donji bitovi
   
   while(1){} // beskonacna petlja
}
