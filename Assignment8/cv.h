#ifndef CV_H
#define CV_H


#include "spinlock.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>


#define CV_MAXPROC 64 

static int wait_count;
static int wakeup_count;
static int swakeup_count;


typedef struct cv{
	int counter;
	account lock;
	pid_t pid_list[CV_MAXPROC];
}cv;


/*initialize any internal data structures in cv so that it is ready for
 * use. The initial condition is that nobody is waiting for this cv. 
 * You can probably arrange your struct cv so that all-0 bytes is
 * te initialization condition.
 */
void cv_init(struct cv *cv);



/*This will be called with the spinlock mutex held by the caller (otherwise 
 * results will be undefined). Atomically record within the internals 
 * of cv that the caller is going to sleep, release the mutex, and
 * go to sleep (see text below). After waking up, re-acquire the mutex
 * before returning to the caller
 */
void cv_wait(struct cv *cv, struct account *mutex);



/*Wake up any and all waiters (sleepers) on this cv. If there are no waiters 
 * the call has no effect and is not "remembered" for the next time that
 * someone calls cv_wait. cv_broadcast should be called with the same mutex 
 * held that protects cv_wait, as discussed in lecture notes under "Lost
 * wWakeup", but note that cv_broadcast does not take a mutex as a parameter. 
 * Return value: the number of sleepers that were awoken
 */
int cv_broadcast(struct cv *cv);




/* Exactly the same as cv_broadcast except at most one sleeper is awoken. 
 * Your choice how to pick which one if more than one candidate
 */
int cv_signal(struct cv *cv);



#endif
