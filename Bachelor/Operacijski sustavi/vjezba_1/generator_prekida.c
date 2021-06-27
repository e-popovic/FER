#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int pid=0;

void prekidna_rutina(int sig){
	kill(pid,SIGKILL);
	exit(0);
}

int main(int argc, char *argv[]){
	pid=atoi(argv[1]);
	sigset(SIGINT, prekidna_rutina);

	while(1){
		srand(time(0));		
		int f=(rand()%(5-3+1))+3;	/*(rand()%(gornja - donja +1))+ donja*/	
		sleep(f);
		int e=rand()%5;
		switch(e){
			case 0:
				kill(pid,SIGUSR1);
				break;
			case 1:
				kill(pid,SIGUSR2);
				break;
			case 2:
				kill(pid,SIGCHLD);
				break;
			case 3:
				kill(pid,SIGABRT);
				break;
			case 4:
				kill(pid,SIGINT);
				break;
		}
		
	}
	return 0;
}
