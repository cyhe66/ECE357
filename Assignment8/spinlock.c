#include "spinlock.h"
#include <sched.h>

void spin_lock(struct account *l){
	while ((tas(&(l->lock)))!= 0){
		sched_yield(); 
	}
	//when we get here, we are the only task that could be accessing
}

void spin_unlock(struct account *l){
	l->lock = 0;
}

