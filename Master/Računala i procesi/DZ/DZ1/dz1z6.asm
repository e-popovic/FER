		LIST p=18f1220 ; ciljni procesor
		CONFIG PWRT = ON, WDT = OFF, MCLRE = OFF
		;konfiguracija mikrokontrolera
		;WDT = OFF = iskljuci watchdog timer
		;PWRT = ON = ukljuci Power Up Timer
		;MCLRE = OFF = interno spoji ~MCLR na Vcc
		#include "P18f1220.INC" ; datoteka s definicijama konstanti za nas PIC

Ram   	EQU h'0' ; rezervacija okteta za varijable
brzina 	EQU 0x10 ; lokacija za upis oznake brzine (0 / FF)

		org 0 ; obavezno
		goto start	; postavljanje pocetnih vrijednosti, normalan rad
		
		org 8 ; prekid
		btfss INTCON3, 1 ; ako prekid nije od INT2 vrati u beskonacnu petlju
		goto kraj
		
		bsf ADCON0, GO ; pokreni novu pretvorbu
pretvori btfsc ADCON0,GO ; je li pretvorba gotova
		goto pretvori ; vrti dok traje pretvorba
	   
		btfsc ADRESH, 1 ; je li najvisi bit 1? (brzina > 512 ?)
		goto brza	; skoci na obradu prevelike brzine
		btfsc ADRESH, 0 ; je li drugi najvisi bit 1? (brzina > 256 ?)
		goto brza ; skoci na obradu prevelike brzine
		movlw d'178' ; 178 (izracunata oznaka za 60km/h) u W
		subwf ADRESL, 0	; usporedi ocitanu brzinu s 60kmh
		btfss STATUS, C ; je li brzina < 60kmh
		goto spora ; (je) skoci na obradu male brzine
		btfsc STATUS, Z ; je li brzina = 60kmh 
		goto spora ; (je) skoci na obradu male brzine
		
brza	movlw b'00010000' ; maska – paljenje LED
		movwf LATB ; aktiviraj LED 
		movlw 0x10 ; lokacija za upis podatka o brzini je 0x10
		movwf BSR ; odaberi 0x10 kao lokaciju
		movlw 0xFF ; sve jedinice
		movwf brzina ; upisi jedinice na lokaciju
		goto gotovo ; zavrsi prekid

spora	movlw b'00000000' ; maska – ne gori LED
		movwf LATB ; deaktiviraj LED 
		movlw 0x10 ; lokacija za upis podatka o brzini je 0x10
		movwf BSR ; odaberi 0x10 kao lokaciju
		movlw 0x00 ; sve nule
		movwf brzina ; upisi nule na lokaciju
		
gotovo	bcf INTCON3, 1 ; ocisti prekidnu zastavicu
		;bsf ADCON0, GO ; pokreni novu pretvorbu
kraj	retfie ; povratak iz prekida, vraćanje konteksta		
		
		
start	clrf LATB ; pocisti port B
		movlw b'00000101' ; maska – ulazi su RB0/AN4 (A/D) i RB2/INT2
		movwf TRISB ; postavi registar smjera GPIO porta B
		movlw b'00010001' ; AD uključen na kanalu AN4
		movwf ADCON0 ; analog input AN4, Fosc/2 za A/D

		movlw b'01101111' ; AN4 je A/D, ostali GPIO
		movwf ADCON1 
		movlw b'10001101' ; Fosc/16, 8 TAD, desno poravnano
		movwf ADCON2 
		
		bsf PIE1, ADIE ; omogući generiranje prekida od A/D jedinice
		;bsf ADCON0, GO ; pokretanje pretvorbe
		
		movlw 0x10       
		movwf INTCON3    ; dozvoli INT2 prekide
		movlw 0x80       
		movwf INTCON     ; dozvoli globalne prekide

loop 	goto loop ; vrti beskonacno
			
		END