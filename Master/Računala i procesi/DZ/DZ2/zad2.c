#include <p18f1220.h>
#pragma config WDT = OFF
#pragma config PWRT= ON
#pragma config MCLRE=OFF // zadano

unsigned int brzina = 0; // globalna varijabla brzine
unsigned int preljev = 0; // koliko puta se dogodio preljev
unsigned mjerenjeBrzine = 0; // je li u tijeku mjerenje brzine

void high_isr(void); // prototip prekida

#pragma code high_vector=0x08 // prekidni vektor

void interrupt_at_high_vector(void){
     _asm GOTO high_isr _endasm // prekidna rutina 
}
#pragma code // gotov prekidni vektor

#pragma interrupt high_isr 
// prekidna rutina visokog prioriteta
void high_isr (void){
   
   if (PIR1bits.TMR1IF) { // ako se dogodio preljev
      ++preljev; // povecaj brojac preljeva
      PIR1bits.TMR1IF = 0; // postavi indikator prekida nazad na 0
      
      T1CON = 0xC9; // pokreni timer1 otpocetka
      TMR1H = 0; // broji do 0 (65536 taktova)
      TMR1L = 0;
   }
   
   else if (PIR1bits.CCP1IF) { // ako je detektiran pokret
      PIR1bits.CCP1IF = 0; // postavi indikator prekida nazad na 0
      
      if ( !mjerenjeBrzine ) { // ako je auto sada prosao kroz prvo osjetilo
	 mjerenjeBrzine = 1; // pocni mjeriti vrijeme
	 
	 T1CON = 0xC9; // pokreni timer1, 16-bitni, interni oscilator, ne koristi prescaler
	 TMR1H = 0; // broji do 0 (65536 taktova)
	 TMR1L = 0;
      }
      
      else { // ako je auto prosao kroz oba osjetila
	 mjerenjeBrzine = 0; // vise se ne mjeri brzina
	 brzina = 0.02 / ( (float) ( 65536 * preljev + 256 * CCPR1H + CCPR1L  ) / 10000000 ); // izracunaj brzinu -> 2cm / (taktovi / (Fosc / 4 ) )
	 preljev = 0; // ocisti brojac preljeva
      }
   }
   
}
   
#pragma code // gotov prekidni vektor


void main(void){   
   PORTB = 0; // ocisti PORTB
   TRISB = 0x08; // cetvrti bit je ulaz mjerila

   CCP1CON = 0x05; //  capture mode, na rastuci brid 
   T3CON = 0; // timer1 je izvor takta za ccp
   
   PIE1 = 0x05; // dozvoli CCP prekid i preljev prekid
   IPR1 = 0x05; // CCP i preljev prekid imaju visoki prioritet
   INTCON = 0xC0; // dozvoli globalne prekide + prekid zbog preljeva
   
   while(1){} // beskonacna petlja
}
