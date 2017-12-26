#include "spinlock.h"
#include <sched.h>
#include <string.h>

//tas works as follows-- 
//returns 0 when the lock has been acquired
//returns 1 when the lock hsa not been acquired
//

void spin_lock(struct account *l){
	while ((tas(&(l->lock)))!= 0){
		;
	}
	//when we get here, we are the only task that could be accessing
	l->target_num += 1;
}

void spin_unlock(struct account *l){
	l->lock = 0;
}

