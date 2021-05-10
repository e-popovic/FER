#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define TAS(ZASTAVICA) __atomic_test_and_set (&ZASTAVICA, __ATOMIC_SEQ_CST)

unsigned long int A;
char zastavica = 0;

void *dodajM(void *M){
   int j;
   for (j=0;j<*((int*)M);j++){
      while ( TAS(zastavica) != 0 )    ;
      A++;
      zastavica=0;
   }
}

int main(int argc, char *argv[])
{
   int N=atoi(argv[1]);   /*broj dretvi*/
   int M=atoi(argv[2]);   /*kolko puta*/
   int i;
   pthread_t thr_id[N];

   A = 0;

   for (i=0;i<N;i++){
      if (pthread_create(&thr_id[i], NULL, dodajM, &M)) {
         printf("Greska pri stvaranju dretve!\n");
         exit(1);
      }
   }
   
   for (i=0;i<N;i++){
      pthread_join(thr_id[i], NULL);
   }
   
   printf("A = %ld\n",A);   


   return 0;
}

