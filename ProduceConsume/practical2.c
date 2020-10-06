
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "cond.c"

int pnum;  // number updated when producer runs.
int csum;  // sum computed using pnum when consumer runs.
int p_lock; //lock when producer is running
int c_lock; //lock when consumer is running

int (*pred)(int); // predicate indicating number to be consumed

int produceT() {
  if(c_lock==1){ 
	  p_lock=1;//p gets lock
	  c_lock=0;//c loses lock
	  scanf("%d",&pnum);
	  
  }
  return pnum;
}

void *Produce(void *a) {
  int p;

  p=1;
  while (p) {
   /* printf("producer thinking...\n");
    sleep(1);
    printf("..done!\n");*/
    p = produceT();
    printf("PRODUCED %d\n",p);
  }
  printf("EXIT-P\n");
 
}


int consumeT() {
  
  if(p_lock==1){
	c_lock=1;//cons gets lock
	p_lock=0;//prod gets lock
	if ( pred(pnum) && pnum!=999 ) { csum += pnum; } //check that its not the fisrt val
   	
  }
  return pnum;
}

void *Consume(void *a) {
  int p;

  p=1;
  while (p) {
    /*printf("consumer thinking...\n");
    sleep(rand()%3);
    printf("..done!\n");*/
    p = consumeT();
    printf("CONSUMED %d\n",csum);
  }
  printf("EXIT-C\n");
 
}


int main (int argc, const char * argv[]) {
  // the current number predicate
  static pthread_t prod,cons;
	long rc;
	p_lock=1; //lock to begin
	c_lock=0; // unlock to begin

  pred = &cond1;
  if (argc>1) {
    if      (!strncmp(argv[1],"2",10)) { pred = &cond2; }
    else if (!strncmp(argv[1],"3",10)) { pred = &cond3; }
  }


  pnum = 999;
  csum=0;
  srand(time(0));

  printf("Creating Producer:\n");
 	rc = pthread_create(&prod,NULL,Produce,(void *)0);
	if (rc) {
			printf("ERROR return code from pthread_create(prod): %ld\n",rc);
			exit(-1);
		}
  printf("Creating Consumer:\n");

 	rc = pthread_create(&cons,NULL,Consume,(void *)0);
	if (rc) {
			printf("ERROR return code from pthread_create(cons): %ld\n",rc);
			exit(-1);
		}

	pthread_join( prod, NULL);
	pthread_join( cons, NULL);


  printf("csum=%d.\n",csum);

  return 0;
}
