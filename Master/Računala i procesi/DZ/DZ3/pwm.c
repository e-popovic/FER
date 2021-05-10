#include <p18f2320.h>
#include <timers.h>
#include <pwm.h>
#include <delays.h>

#pragma config WDT = OFF
#pragma config PWRT= ON
#pragma config MCLRE=OFF

#define N 25; // N i A = 25
unsigned int sinus[] = { 25, 31, 37, 42, 46, 48, 49, 49, 47, 44, 39, 34, 28, 21, 15, 10, 5, 2, 0, 0, 1, 3, 7, 12, 18 }; // Lookup tablica vrijednosti sinusa
unsigned int i = 0; // Vrijednost popunjenosti

void main(void) {
   OpenPWM1( 49 ); // PR2 = 49 = 2*A - 1
   
   OpenTimer2(TIMER_INT_OFF & T2_PS_1_4); // Ukljuci timer2, preddjelilo = 4 
   
   while(1) {
      
      SetDCPWM1(sinus[i]<<2); // Postavi popunjenost(velicina 10b !)
      i = (i + 1)%N; // Pomakni indeks u tablici popunjenosti (cirkularno)
      
      Delay100TCYx(96); // cekanje 9600 taktova
      Delay10TCYx(5); // cekanje 50 taktova
   } 
   
   ClosePWM1(); // Iskljuci PWM
   CloseTimer2(); // Iskljuci Timer2
}