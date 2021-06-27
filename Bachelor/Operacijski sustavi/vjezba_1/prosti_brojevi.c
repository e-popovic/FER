#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

unsigned long pauza=0;
unsigned long broj=1000000001;
unsigned long zadnji=1000000001;

int jelprost(unsigned long n){
	unsigned long i, max;
	if ((n&1) == 0) return 0;
	max = sqrt(n);
	for ( i = 3; i <= max; i += 2 )
		if ( ( n % i ) == 0 )
			return 0;
	return 1;
}

void postavi_pauzu(){
	pauza=1-pauza;
}

void periodicki_ispis(){
	printf("zadnji prosti broj = %ld\n", zadnji);
}

void prekini(void){
	printf("zadnji prosti broj = %ld\n", zadnji);
	exit(0);
}

int main(void){
	struct itimerval t;	
	sigset(SIGTERM, prekini );
	sigset(SIGINT, postavi_pauzu );

	/* povezivanje obrade signala SIGALRM sa funkcijom "periodicki_posao" */
	sigset ( SIGALRM, periodicki_ispis );

	/* definiranje periodičkog slanja signala */
	/* prvi puta nakon: */
	t.it_value.tv_sec = 0;
	t.it_value.tv_usec = 500000;  /* 5 sekundi= 5 * 10^6 */
	/* nakon prvog puta, periodicki sa periodom: */
	t.it_interval.tv_sec = 0;
	t.it_interval.tv_usec = 500000;

	/* pokretanje sata s pridruženim slanjem signala prema "t" */
	setitimer ( ITIMER_REAL, &t, NULL );
	
	
	while(1){
		if (jelprost(broj)==1)
			zadnji=broj;
		++broj;
		while(pauza==1){
			pause();
		}
	}
	return 0;
}

