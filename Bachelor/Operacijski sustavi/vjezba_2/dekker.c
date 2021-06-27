#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
int Id; /* identifikacijski broj segmenta */
unsigned long int *A;
int PRAVO;
int ZASTAVICA[2];

void dodajM(int M,int l){
   
   int i,j;
   if (l==0){
      i=0;
      j=1;
   }
   if (l==1){
      i=1;
      j=0;
   }
   int k;   
   for (k=0;k<M;k++){
      udi_u_kriticni_odsjecak(i,j);
      (*A) +=1;
      izadi_iz_kriticnog_odsjecka(i,j);
      
   }
}

void udi_u_kriticni_odsjecak(int i,int j)
{
   ZASTAVICA[i] = 1;
   while (ZASTAVICA[j]>0) {
      if (PRAVO==j) {
         ZASTAVICA[i] = 0;
         while (PRAVO==j);
         ZASTAVICA[i] = 1;
      }
   }
}
 
void izadi_iz_kriticnog_odsjecka(int i,int j)
{
   PRAVO = j;
   ZASTAVICA[i] = 0;
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
   int M=atoi(argv[1]);   /*kolko puta*/
   int i;
   /* zauzimanje zajedničke memorije */
   Id = shmget(IPC_PRIVATE, sizeof(int), 0600);   /*ostavila*/
 
   if (Id == -1)
      exit(1);  /* greška - nema zajedničke memorije */   /*ostavila*/
    
   A = (int *) shmat(Id, NULL, 0);   /*ostavila*/
   *A = 0;
   sigset(SIGINT, brisi);//u slučaju prekida briši memoriju


 
   /* pokretanje paralelnih procesa */   

   for (i = 0; i < 2; i++){
   switch (fork()) {
   case 0:
      dodajM(M,i);
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
