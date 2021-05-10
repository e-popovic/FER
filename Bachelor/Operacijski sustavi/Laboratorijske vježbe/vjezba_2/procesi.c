#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
int Id; /* identifikacijski broj segmenta */
unsigned long int *A;
 
void dodajM(int M){
   int j;
   for (j=0;j<M;j++)
      (*A) +=1;

}

void brisi(int sig)
{
   /* oslobađanje zajedničke memorije */
   (void) shmdt((char *) A);
   (void) shmctl(Id, IPC_RMID, NULL);
   exit(0);
}
int main(int argc, char *argv[])
{
   int N=atoi(argv[1]);   /*broj procesa*/
   int M=atoi(argv[2]);   /*kolko puta*/
   int i;
   /* zauzimanje zajedničke memorije */
   Id = shmget(IPC_PRIVATE, sizeof(int), 0600);   /*ostavila*/
 
   if (Id == -1)
      exit(1);  /* greška - nema zajedničke memorije */   /*ostavila*/
    
   A = (int *) shmat(Id, NULL, 0);   /*ostavila*/
   *A = 0;
   sigset(SIGINT, brisi);//u slučaju prekida briši memoriju
 
   /* pokretanje paralelnih procesa */   

   for (i = 0; i < N; i++){
   switch (fork()) {
   case 0:
      dodajM(M);
      exit(0);
   case -1:
      printf("Greska ");
      exit(1);
   default:
      break;    
   }}
 
   while (i--) wait (NULL);

   printf("A = %d\n",*A);    
   brisi(0);
 
   return 0;
}
