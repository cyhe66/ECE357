#ifndef SPINLOCK_H
#define SPINLOCK_H

typedef struct account {
	char lock; //0 if unlocked, NZ is locked
	int target_num;
} account;

int tas(volatile char* lock);
void spin_lock(struct account *l);
void spin_unlock(struct account *l);

#endif
