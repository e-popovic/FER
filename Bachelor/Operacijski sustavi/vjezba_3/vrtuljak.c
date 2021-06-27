#include <stdio.h>					
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <signal.h>
int SemId;   /* identifikacijski broj skupa semafora */


void SemGet(int n){  /* dobavi skup semafora sa ukupno n semafora */
	SemId = semget(IPC_PRIVATE, n, 0600);
	if (SemId == -1) {
		printf("Nema semafora!\n");
		exit(1);
	}
}
int SemOp(int SemNum, int SemOp){  /* obavi operaciju SemOp sa semaforom SemNum */
	struct sembuf SemBuf;
	SemBuf.sem_num = SemNum;
	SemBuf.sem_op  = SemOp;
	SemBuf.sem_flg = 0;
	return semop(SemId, & SemBuf, 1);
}

void posjetitelj(int w) {

	if (SemOp( 0, -1) == -1 ){
		printf("Greska ");
		exit(1);		//Ispitaj_OSEM[0];
	} 		 		//jel ima mjesta za sjest
	
	printf("Ulazi posjetitelj br %d \n",w);
	sleep(1);

	if (SemOp( 1, 1) == -1 ){
		printf("Greska ");
		exit(1);		//Postavi_OSEM[1];
	} 				//posjetitelj ulazi
	  
	//vrti see
	if (SemOp( 2, -1) == -1 ){
		printf("Greska ");
		exit(1);		//Cekaj_OSEM[2];
	} 		 		//kraj

	printf("Izlazi posjetitelj br %d \n",w);
	sleep(1);

	if (SemOp( 3, 1) == -1 ){
		printf("Greska ");
		exit(1);		//Postavi_OSEM[3] ;
	}				//ustani i pokupi se
	
}

void vrtuljak(int N) {
	int i=0;
	while(1) {
		for (i=0;i<N;i++){
				if (SemOp( 0, 1) == -1 ){
					printf("Greska ");
					exit(1);		//Postavi_OSEM[0];
				}				//moze se sjest
		}
		for (i=0;i<N;i++){
				if (SemOp( 1, -1) == -1 ){
					printf("Greska ");
					exit(1);		//Ispitaj_OSEM[1];
				}				//dal su svi usli
		}
		sleep(1);
		
		printf("Vrtuljak krece \n");
		sleep(3);

		printf("Vrtuljak staje \n");
		sleep(3);

		for (i=0;i<N;i++){
				if (SemOp( 2, 1) == -1 ){
					printf("Greska ");
					exit(1);		//Postavi_OSEM[2];
				}				//kraj

		}
		
		for (i=0;i<N;i++){
				if (SemOp( 3, -1) == -1 ){
					printf("Greska ");
					exit(1);		//Ispitaj_OSEM[3];
				}				//cekaj da svi odu
		}

   }
}


int SemSetVal(int SemNum, int SemVal){  /* postavi vrijednost semafora SemNum na SemVal */
	return semctl(SemId, SemNum, SETVAL, SemVal);
}
void SemRemove(void){  /* uništi skup semafora */
	(void) semctl(SemId, 0, IPC_RMID, 0);
}

void exit_signal(void){  
	SemRemove();
	exit(1);
}


void main(int argc, char *argv[]){
	sigset(SIGINT, exit_signal);

	if(argc != 3){
		printf("Greska u inputu");
		exit(1);
	}
	
	int N = atoi(argv[1]);
	printf("broj sjedala N = %d\n", N);
	int M = atoi(argv[2]);
	printf("broj posjetitelja = %d\n", M);

	SemGet(4);
	SemSetVal(0, 0);	//jesu li mjesta slobodna?
	SemSetVal(1, 0);	//jesu li svi usli? / ulazak
	SemSetVal(2, 0);	//zavrsila voznja
	SemSetVal(3, 0);	//jesu li svi sišli? / silazak
	
	
	switch(fork()){
		case -1:
			printf("Greska u stvaranju procesa\n");
			exit_signal();	
			break;
		case 0:
			vrtuljak(N);
			exit(0);
			break;
		default:
			break;
		}
	int i;
	for (i=0;i<M;i++){
		switch(fork()){
		case -1:
			printf("Greska u stvaranju procesa %d\n", i);
			exit_signal();	
			break;
		case 0:
			posjetitelj(i+1);
			exit(0);
			break;
		default:
			break;
		}
		
	}
	i++;
	while (i--) wait (NULL);

	SemRemove();
	

	return 0;
	
}

