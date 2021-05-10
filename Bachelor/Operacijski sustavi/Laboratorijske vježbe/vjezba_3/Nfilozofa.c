#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <malloc.h>
int br_jela;
int br_stapica;
int br_filozofa;
int *br_ruku;
pthread_cond_t red;
pthread_mutex_t m;
FILE *file;

void uzeti_stapice(int i){
	pthread_mutex_lock(&m);
	while (br_stapica < br_ruku[i]){
		pthread_cond_wait(&red,&m);
	}
	int j;
	for (j=0;j<br_ruku[i];j++){
		br_stapica--;	
	}
	pthread_mutex_unlock(&m);
}

void vratiti_stapice(i){
	pthread_mutex_lock(&m);
	int j;
	for (j=0;j<br_ruku[i];j++){
		br_stapica++;	
	}
	pthread_cond_broadcast(&red);	
	pthread_mutex_unlock(&m);
}

void *Filozof(int i){
	int k;
	for (k=0;k<br_jela;k++){
		printf("%d. filozof misli.\n",i+1);
		sleep(1);
		uzeti_stapice(i);
		printf("%d. filozof s %d ruke jede.\n	%d stapica na stolu\n",i+1,br_ruku[i],br_stapica);
		sleep(2);
		vratiti_stapice(i);
		printf("%d. filozof spusta stapice.\n	%d stapica na stolu\n",i+1,br_stapica);
	}
}

void exit_signal(void){  
	fclose(file);
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&red);
	free(br_ruku);
	exit(2);
}

int main(int argc, char *argv[]){
	sigset(SIGINT, exit_signal);


	if(argc != 2){
		printf("Greska u inputu");
		exit(1);
	}
	
	br_jela = atoi(argv[1]);
	printf("Svatko jede %d puta.\n",br_jela );
	

	file=fopen("filozofi.txt","r");	
	if (file==NULL){
		printf("Ne postoji file");
		exit(1);
	}
	fscanf(file, "%d %d", &br_stapica, &br_filozofa);
	br_ruku = (int*)malloc(sizeof(int));
	int i;
	for (i = 0; i < br_filozofa; i++){
		if(fscanf(file, "%d", &br_ruku[i]) == EOF){
			printf("Nedovoljno podataka u fileu.\n");
			exit_signal();
		}
	}
	printf("Broj filozofa: %d\n",br_filozofa);
	printf("Broj stapica: %d\n",br_stapica);

	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&red, NULL);

	pthread_t thr_id[br_filozofa];
	for (i=0;i<br_filozofa ;i++){
		if (pthread_create(&thr_id[i], NULL, (void*)(*Filozof), i)) {	//i bez &?
			printf("Greska pri stvaranju dretve!\n");
			exit(1);
		}
	}

	for (i=0;i<br_filozofa;i++){
		pthread_join(thr_id[i], NULL);
	}

	fclose(file);
	pthread_cond_destroy(&red);
	pthread_mutex_destroy(&m);
	free(br_ruku);

	return 0;	
}
