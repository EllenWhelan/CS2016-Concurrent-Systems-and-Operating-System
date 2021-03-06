// ONLY MODIFY CODE BELOW THE NEXT OCCURENCE OF THE FOLLOWING LINE !
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "types.h"
#include "defs.h"
#include <stdio.h>
#include "proc.h"

#define NCPU 1

struct cpu cpus[NCPU];
int ncpu;

void printstate(enum procstate pstate){ // DO NOT MODIFY
  switch(pstate) {
    case UNUSED   : printf("UNUSED");   break;
    case EMBRYO   : printf("EMBRYO");   break;
    case SLEEPING : printf("SLEEPING"); break;
    case RUNNABLE : printf("RUNNABLE"); break;
    case RUNNING  : printf("RUNNING");  break;
    case ZOMBIE   : printf("ZOMBIE");   break;
    default       : printf("????????");
  }
}

// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.

// local to scheduler in xv6, but here we need them to persist outside,
// because while xv6 runs scheduler as a "coroutine" via swtch,
// here swtch is just a regular procedure call.
struct proc *p;
struct cpu *c = cpus;

// +++++++ ONLY MODIFY BELOW THIS LINE ++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void
scheduler(void)
{ int runnableFound; // DO NOT MODIFY/DELETE
	int i;
	struct proc * min_proc =&ptable.proc[0]; //intitialise to 0

	//array to keep track of how many runs each process has had and initialises at zero
	int process_runs[NPROC];
	for( i=0; i<NPROC;i++){
		process_runs[i]=0;
	}

  c->proc = 0;

  runnableFound = 1 ; // force one pass over ptable
  
  while(runnableFound){ // DO NOT MODIFY
    // Enable interrupts on this processor.
    // sti();
	struct proc * min_proc =&ptable.proc[0]; //intitialise to 0
    runnableFound = 0; // DO NOT MODIFY
    // Loop over process table looking for process to run.
    // acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
	//for loop to find min process 
	for(i=0; i<NPROC; i++){
		struct proc * q =&ptable.proc[i];
		if(process_runs[i] < process_runs[min_proc->pid] && q->state != SLEEPING && q->state != UNUSED){
			min_proc=&ptable.proc[i];
		}
	}
	
      if(p->state != RUNNABLE || process_runs[p->pid] != process_runs[min_proc->pid])
        continue;
	
      runnableFound = 1; // DO NOT MODIFY/DELETE/BYPASS

      // Switch to chosen process.  It is the process's job
      // to release ptable.lock and then reacquire it
      // before jumping back to us.
      c->proc = p;
      
      //switchuvm(p);
      p->state = RUNNING;
      process_runs[p->pid]++; //incrementing the number that proc has run

      swtch(p);
      // p->state should not be running on return here.
      //switchkvm();
      // Process is done running for now.
      // It should have changed its p->state before coming back.
      c->proc = 0;
    }
    // release(&ptable.lock);

  }
  printf("No RUNNABLE process!\n");
}
