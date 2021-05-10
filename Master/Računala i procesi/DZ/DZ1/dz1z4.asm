      LIST p=18f1220 ; ciljni procesor
       CONFIG PWRT = ON, WDT = OFF, MCLRE = OFF
       ;konfiguracija mikrokontrolera
       ;WDT = OFF = iskljuci watchdog timer
      ;PWRT = ON = ukljuci Power Up Timer
      ;MCLRE = OFF = interno spoji ~MCLR na Vcc
       #include "P18f1220.INC" ; datoteka s definicijama konstanti za nas PIC

Ram   EQU h'0' ; rezervacija okteta za varijable
state EQU Ram+0 ; registri opce namjene pocinju od adrese 0h
l1    EQU Ram+1
l2    EQU Ram+2

     org 0 ; obavezno
     clrw ; resetiraj registar W
     clrf LATB ; pocisti port B
     clrf TRISB ; sve linije izlazne

     clrf ADCON0 ; iskljuci A/D
     movlw 0xff
     movwf ADCON1 ; 0xff, svi ulazi su digitalni

     clrf state ; obrisi stanje

loop call getmask ; izracunaj bit-masku za trenutno stanje
     movwf LATB ; zapisi masku na port
     incf state,W ; povecaj stanje, rezultat stavi u W
     andlw 0x07 ; uzmi samo potrebne bitove
     movwf state ; spremi stanje natrag u memoriju
     call wait ; cekaj
     goto loop ; ponovi

     ; funkcija vraca bitovnu masku za PORTA, za trenutno stanje.
     ; donja tri bita upravljaju jednim semaforom, gornja drugim
     ; bit1 je crveno, bit2 narancasto a bit3 zeleno svjetlo
getmask movf state, W ; dohvati stanje u registar W.
     addwf PCL,1 ; dodaj W registru PCL, rezultat u PCL.
     retlw 0x21 ; 010 0001 ; state==0 upaljeno zeleno i crveno.
     retlw 0x03 ; 000 0011 ; state==1 upaljeno narancasto i crveno/narancasto
     retlw 0x14 ; 001 0100 ; state==3 upaljeno crveno i zeleno
     retlw 0x12 ; 001 0010 ; state==4 upaljeno crveno/narancasto i narancasto.

     ; funkcija wait ostvaruje kasnjenje pomocu dvije petlje.
wait movlw 5
     movwf l1
w1 call wait2
   decfsz l1
   goto w1
   return

wait2 clrf l2
w2 decfsz l2
   goto w2
   return
   END
