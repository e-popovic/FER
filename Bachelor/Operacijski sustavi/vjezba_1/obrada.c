#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define N 6    /* broj razina proriteta */
 
int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET;

int sig[]={SIGUSR1, SIGUSR2, SIGCHLD, SIGABRT, SIGINT};

void zabrani_prekidanje(){
	int i;
	for(i=0; i<N; i++)
	sighold(sig[i]);
}
void dozvoli_prekidanje(){
	int i;
	for(i=0; i<N; i++)
		sigrelse(sig[i]);
}
 
void obrada_prekida(int i){
	int k;
	for (k=0;k<=6;k++){
		switch(sig[i-1]){
			case SIGUSR1:
				if (k==0) printf(" -  P  -  -  -  -\n");
				else if (k==6) printf(" -  K  -  -  -  -\n");
				else printf(" -  %d  -  -  -  -\n",k);
				break;
			case SIGUSR2:
				if (k==0) printf(" -  -  P  -  -  -\n");
				else if (k==6) printf(" -  -  K  -  -  -\n");
				else printf(" -  -  %d  -  -  -\n",k);
				break;
			case SIGCHLD:
				if (k==0) printf(" -  -  -  P  -  -\n");
				else if (k==6) printf(" -  -  -  K  -  -\n");
				else printf(" -  -  -  %d  -  -\n",k);
				break;
			case SIGABRT:
				if (k==0) printf(" -  -  -  -  P  -\n");
				else if (k==6) printf(" -  -  -  -  K  -\n");
				else printf(" -  -  -  -  %d  -\n",k);
				break;
			case SIGINT:
				if (k==0) printf(" -  -  -  -  -  P\n");
				else if (k==6) printf(" -  -  -  -  -  K\n");
				else printf(" -  -  -  -  -  %d\n",k);
				break;
		}
		sleep(1);		/**/
	}
}
void prekidna_rutina(int sig){
	int n=-1;
	zabrani_prekidanje();
	switch(sig){
		case SIGUSR1: 
			n=1; 
			printf(" -  X  -  -  -  -\n");
			break;
		case SIGUSR2: 
			n=2; 
			printf(" -  -  X  -  -  -\n");
			break;
		case SIGCHLD: 
			n=3; 
			printf(" -  -  -  X  -  -\n");
			break;
		case SIGABRT: 
			n=4; 
			printf(" -  -  -  -  X  -\n");
			break;
		case SIGINT: 
			n=5; 
			printf(" -  -  -  -  -  X\n");
			break;
	}
	OZNAKA_CEKANJA[n]++;
	int x;
	do{
		x=0;
		int j;
		for (j=TEKUCI_PRIORITET+1;j<N;j++){		/**/
			if (OZNAKA_CEKANJA[j]>0) x=j;  /*sign najveceg prioriteta*/
		}
		if (x>0){
			OZNAKA_CEKANJA[x] --;			
			PRIORITET[x] = TEKUCI_PRIORITET;
			TEKUCI_PRIORITET = x;			
			dozvoli_prekidanje();
			obrada_prekida(x);		/*sign prioritetniji od onog trenutnog? obradi! */	/**/
			zabrani_prekidanje();
			TEKUCI_PRIORITET = PRIORITET[x];		
		}	
	}while(x>0);
	dozvoli_prekidanje();
 }
 
int main ( void )
{
	sigset (SIGUSR1, prekidna_rutina);
	sigset (SIGUSR2, prekidna_rutina);
	sigset (SIGCHLD, prekidna_rutina);
	sigset (SIGABRT, prekidna_rutina);
	sigset (SIGINT, prekidna_rutina);
 
	printf("Proces obrade prekida, PID=%ld\n", getpid());
	printf("GP S1 S2 S3 S4 S5\n-----------------\n");
	int a;
	for (a=1;a>0;++a){
		printf(" %2d  -  -  -  -  -\n",a);
		sleep(1);
	}
	
 
	printf ("Zavrsio osnovni program\n");
 
	return 0;
}

