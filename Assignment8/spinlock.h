#ifndef SPINLOCK_H
#define SPINLOCK_H

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include "tas64.S"

struct account {
	int lock; //0 if unlocked, NZ is locked
	int target_num;
	//for debugging purposes
	//pid_t current_holder; //current holder of the spin_lock
	//int access_count;//incremented access count
};


int tas64(volatile char*);

void spin_lock(struct account *l);
void spin_unlock(struct account *l);



#endif
