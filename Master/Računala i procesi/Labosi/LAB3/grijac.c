#include <p18f2320.h>
#include <timers.h>
#include <pwm.h>
#include <delays.h>

#pragma config WDT = OFF
#pragma config PWRT= ON
#pragma config MCLRE=OFF

int prosla_greska = 0;	
int greska = 0;
int izlaz = 0;
int I_komponenta = 0;	// komponenta potrebna - I dio
int D_komponenta = 0;	// komponenta potrebna - D dio
float Kp = 19.75;	// konstanta - P dio
float Ki = 0.005;	// konstanta - I dio
int Kd = 70;	// konstanta - D dio
int i_help = 0;
int help = 0;

void high_isr(void); // prototip prekida

#pragma code high_vector=0x08 // prekidni vektor

void interrupt_at_high_vector(void){
     _asm GOTO high_isr _endasm // prekidna rutina 
}
#pragma code // gotov prekidni vektor

#pragma interrupt high_isr 
// prekidna rutina visokog prioriteta
void high_isr (void){
   
   if (INTCONbits.TMR0IF) { // ako se dogodio preljev zbog odbrojenog timera 0 - pocni pretvorbu temperature
      INTCONbits.TMR0IF = 0; // postavi indikator prekida nazad na 0
      ADCON0bits.GO = 1;	// pocni A/D konverziju brzine      
   }
   
   else if (PIR1bits.ADIF) {	// ako se zavrsila pretvorba temperature
      PIR1bits.ADIF = 0; // postavi A/D pretvorbu nazad na 0
      
      greska = -((ADRESH<<8 + ADRESL) - 256) * 5;	// // (ovo i dolje) - prebaci izmjerenu vrijednost iz registara u volte
      help = greska & 0x8000;
      greska >>= 10;
      if (help){
	 greska |= (0xffff << 6);
      }
      
      i_help = greska>>8;	// pomnozi gresku[V] sa vremenom dt = 2^(-8)
      if (help){
	 i_help |= (0xffff << 8);
      }
      I_komponenta = I_komponenta + i_help;
      
      D_komponenta = (greska - prosla_greska)<<8;	// dijeljenje razlike vremenom 0.00390625 = 2^(-8)

      izlaz = (int) ((Kp * (float)greska)) + (int)(Ki* (float)I_komponenta) + Kd * D_komponenta;	// racun konacne vrijednosti 
      SetDCPWM1(izlaz<<2); // Postavi popunjenost(velicina 10b !)

      prosla_greska = greska;
      
      T0CON = 0x88; // pokreni timer0 otpocetka
      TMR0H = 0xF0; // postavljamo trajanje od 3.906ms => 65536 - 3906 = 61630
      TMR0L = 0xBE;
   }
}
   
#pragma code // gotov prekidni vektor

void main(void) {
   //ulazi i izlazi
   TRISA = 0x01;// sve A linije izlazne osim RA0 - napon peći (A/D)
   TRISB = 0;	// sve B linije izlazne
   TRISC = 0;	// sve C linije izlazne - RC2 - PWM signal
   
   //A/D
   ADCON0 = 0x01; // omoguci A/D s AN0
   ADCON1 = 0x0E; //  citaj s AN0
   ADCON2 = 0x8D; // Fosc/16, 8 TAD, desno poravnano
   PIE1bits.ADIE = 1; // omogući generiranje prekida od A/D jedinice

   //timer0
   INTCON = 0xE0; // dozvoli globalne prekide i prekid timera 0 i AD
   T0CON = 0x88; //  pokreni timer0, 16-bitni, interni oscilator, ne koristi prescaler
   TMR0H = 0xF0; // postavljamo trajanje od 3.906ms => 65536 - 3906 = 61630
   TMR0L = 0xBE; //  donji bitovi
   
   //PWM
   OpenPWM1( 100 ); // PR2 = 100
   OpenTimer2(TIMER_INT_OFF & T2_PS_1_4); // Ukljuci timer2, preddjelilo = 4 
     
   while(1){} // beskonacna petlja
   
   ClosePWM1(); // Iskljuci PWM
   CloseTimer2(); // Iskljuci Timer2
}