#include <spinlock.h>
#include <sched.h>
#include <string.h>
//tas works as follows-- 
//zero value mean unlocked and tas returns the "previous" vlaue of *lock
//returns 0 when the lock has been acquired
//returns 1 when the lock hsa not been acquired
//

void spin_lock(struct account *l){
	int temp;
	while (temp = (tas64(&l->lock)!= 0)){
		//int sched_yield();
		;
	}
	//when we get here, we are the only task that could be accessing
	l->target_num += 1;
	//l->access_count += 1;
	//l->current_holder = getpid();
}

void spin_unlock(struct account *l){
	l->lock = 0;
}

