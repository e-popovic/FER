      LIST p=18f1220 ; ciljni procesor
      CONFIG PWRT = ON, WDT = OFF, MCLRE = OFF
      ;konfiguracija mikrokontrolera
      ;WDT = OFF = iskljuci watchdog timer
      ;PWRT = ON = ukljuci Power Up Timer
      ;MCLRE = OFF = interno spoji ~MCLR na Vcc
      #include "P18f1220.INC" ; datoteka s definicijama konstanti za nas PIC

Ram   EQU h'0' ; rezervacija okteta za varijable
state EQU Ram+0 ; registri opce namjene pocinju od adrese 0h
i    EQU Ram+1
j    EQU Ram+2
brojac EQU Ram+3 ; brojac na pocetku

      org 0 ; obavezno
      goto start ; pokreni prog
     
      org 8 ; prekid
      btfss INTCON3, 1 ; ako prekid nije od INT2 vrati u beskonacnu petlju
      goto kraj
      
      clrf i ; obrisi vrijednost i 
      incf i ; upisi vrijednost 1 u i
      
      bcf INTCON3, 1 ; ocisti prekidnu zastavicu
kraj	retfie ; povratak iz prekida, vraćanje konteksta

     
start movlw 0xFF ; iskljuci A/D
      movwf ADCON1 ; 0xff, svi ulazi su digitalni
      movlw d'0' ; resetiraj registar W
      clrf LATB ; pocisti port B
      clrf TRISB ; sve linije izlazne
      clrf ADCON0 ; iskljuci A/D
      clrf state ; obrisi stanje
      
      movlw 0x0C  ; kasnjenje za semaforom A
      movwf brojac    ; brojac kasnjenja semafora B
      
      movlw 0x10       
      movwf INTCON3    ; dozvoli INT2 prekide
      movlw 0x80       
      movwf INTCON     ; dozvoli globalne prekide
      
loop call delay ; cekaj
      movlw d'0' ; W=0
      subwf brojac, 1 ; brojac = brojac - 0
      btfsc STATUS, Z ; ako brojac nije 0 smanji ga i mijenjaj A, ako je 0, mijenjaj oba
      goto oba ; skoci na mijenjanje svijetla
      movlw d'1' ; W=1
      subwf brojac, 1 ; smanji brojac za 1
      call getmaskA ; izracunaj bit-masku za trenutno stanje
      goto dalje ; posalji na port sto se treba (A ili A+B)

oba call getmaskAB ; izracunaj bit-masku za trenutno stanje

dalje movwf LATB ; zapisi masku na port
      incf state,W ; povecaj stanje, rezultat stavi u W
      andlw 0x03 ; uzmi samo potrebne bitove
      movwf state ; spremi stanje natrag u memoriju
      goto loop ; ponovi

     ; funkcija vraca bitovnu masku za PORTA, kada oba semafora rade
     ; najnizi bit - zeleno, iduci bit - crveno
getmaskAB movf state, W ; dohvati stanje u registar W.
      addwf PCL,1 ; dodaj W registru PCL, rezultat u PCL.
      retlw 0x03 ; 000 0011 ; state==0 upaljeno A i B.
      retlw 0x00 ; 000 0000 ; state==1 ugaseno
      
      ; funkcija vraca bitovnu masku za PORTA, samo za A (pocetne faze)
     ; najnizi bit - zeleno, iduci bit - crveno
getmaskA movf state, W ; dohvati stanje u registar W.
      addwf PCL,1 ; dodaj W registru PCL, rezultat u PCL.
      retlw 0x01 ; 000 0001 ; state==0 upaljeno A, ugaseno B
      retlw 0x00 ; 000 0000 ; state==1 ugaseno


delay movlw 5 ;potprogram za kasnjenje iz DZ-e
      movwf i
w1 call wait2
      decfsz i ; prekidna rutina mijenja ovu varijablu
      goto w1
      return
      
wait2 clrf j
w2  decfsz j
      goto w2
      return 
      
      END
