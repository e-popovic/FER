		LIST p=18f1220 ; ciljni procesor
		CONFIG PWRT = ON, WDT = OFF, MCLRE = OFF
		;konfiguracija mikrokontrolera
		;WDT = OFF = iskljuci watchdog timer
		;PWRT = ON = ukljuci Power Up Timer
		;MCLRE = OFF = interno spoji ~MCLR na Vcc
		#include "P18f1220.INC" ; datoteka s definicijama konstanti za nas PIC

Ram   	EQU h'0' ; rezervacija okteta za varijable
state 	EQU Ram+0 ; registri opce namjene pocinju od adrese 0h

		org 0 ; obavezno
		goto start	; postavljanje pocetnih vrijednosti, normalan rad
	
		org 8 ; prekid
		btfss INTCON3, 1 ; ako prekid nije od INT2 vrati u beskonacnu petlju
		goto kraj
	
		incf state,W ; povecaj stanje, rezultat stavi u W
		andlw 0x07 ; uzmi samo potrebne bitove
		movwf state ; spremi stanje natrag u memoriju
		btfss state, 2	; ako je stanje manje od 4 mijenjaj semafor
		goto svjetla
		btfss state, 1	; ako je stanje manje od 6 mijenjaj semafor
		goto svjetla
		movlw d'0' ; ako je stanje 6 ili 7 stavi ga u 0
		movwf state ; spremi stanje natrag u memoriju	      
svjetla call getmask ; izracunaj bit-masku za trenutno stanje
		movwf LATB ; zapisi masku na porte
	
		bcf INTCON3, 1 ; ocisti prekidnu zastavicu
kraj	retfie ; povratak iz prekida, vraÄ‡anje konteksta


start 	movlw 0xFF ; iskljuci A/D
		movwf ADCON1 ; 0xff, svi ulazi su digitalni
		movlw d'0' ; resetiraj registar W
		clrf LATB ; pocisti port B
		movlw 0x04 ; treci bit je ulazni - promjene stanja
	        movwf TRISB ; omoguci ulaz	
		clrf ADCON0 ; iskljuci A/D
		clrf state ; obrisi stanje
	
		call getmask ; upisi prvo stanje
		movwf LATB ; zapisi masku na port
	
		movlw 0x10       
		movwf INTCON3    ; dozvoli INT2 prekide
		movlw 0x80       
		movwf INTCON     ; dozvoli globalne prekide

loop 	goto loop ; vrti beskonacno

	
getmask	movf state, W ; dohvati stanje u registar W.
		addwf state, 0   ; pomnozi s 2 zbog sirine instrukcije(16 bita)
		addwf PCL, 1 ; dodaj W registru PCL, rezultat u PCL.
		retlw 0x01 ; 000 0001 ; state==0 upaljeno crveno autima i pjesacima.
		retlw 0x21 ; 010 0001 ; state==1 upaljeno crveno autima, zeleno pjesacima
		retlw 0x01 ; 000 0001 ; state==2 upaljeno crveno autima i pjesacima.
		retlw 0x03 ; 000 0011 ; state==3 upaljeno crveno/narancasto autima, crveno pjesacima.
		retlw 0x10 ; 001 0000 ; state==4 upaljeno zeleno autima, crveno pjesacima.
		retlw 0x02 ; 000 0010 ; state==5 upaljeno narancasto autima, crveno pjesacima.

		END

