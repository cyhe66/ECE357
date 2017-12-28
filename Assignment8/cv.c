#include "cv.h"

void handler(int signo){
	//empty signal handler
	//i don't think anything happens here
}


void cv_init(struct cv *cv){
	cv->counter = 0;
	
	for (int ii = 0; ii < CV_MAXPROC; ii++){
		cv->pid_list[ii] = 0;
	}
	
	cv->lock.lock = 0;
	
	signal(SIGUSR1, handler);
	sigfillset(&cv->sigmask);//mask all the defined signals //create the mask
	sigaddset(&cv->sigmask, SIGUSR1); //exclude the SIGUSR1 signal

	/*
	int * addr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, 0, 0);
	if (addr == MAP_FAILED){
		fprintf(stderr,"Failure mmapping: %s", strerror(errno));
		exit(1);
	}

	signal(SIGUSR1, handler);//SIGUSR1 calls the handler
	
	account * spinlock;
	spinlock = (account*)(addr+sizeof(account));

	//all 0 bytes is the initialization condition
	cv->lock = *spinlock;
	for (int ii = 0; ii < CV_MAXPROC; ii++){
		cv->pid_list[ii] = 0;
	}

	cv->counter = 0;
	
	sigfillset(&cv->sigmask);//mask all the defined signals //create the mask
	sigdelset(&cv->sigmask, SIGUSR1); //exclude the SIGUSR1 signal
	*/
}




/*ATOMICALLY {
 * note within *cp that we are interested
 * pthread_mutex_unlock(mut)
 * go to sleep (sig suspend);
 * /////awakened//////////
 * pthread_mutex_lock(mut);
 */

void cv_wait(struct cv *cv, struct account *mutex){
	//lock and unlock 
	spin_lock(&cv->lock);
	cv->pid_list[cv->counter] = (int) getpid();
	cv->counter++;
	spin_unlock(mutex);
	spin_unlock(&cv->lock);

	sigprocmask(SIG_BLOCK, &cv->sigmask, NULL); // block all signals in sigmask 
	sigsuspend(&cv->sigmask);
	spin_lock(mutex);

	/*
	//lock and unlock
	spin_lock(&cv->lock);
	cv->pid_list[cv->counter] = getpid();//take the cur pid and store it
	cv->counter++;
	spin_unlock(&cv->lock);

	spin_unlock(mutex);
	sigprocmask(SIG_BLOCK, &cv->sigmask, NULL); // block all signals in sigmask 
	sigsuspend(&cv->sigmask); //wait for SIGUSR1 to invoke signal handler as trigger to UNsuspend`

	if(cv->counter > 0){ // still waiting
		//lock and unlock
		spin_lock(&cv->lock);
		cv->pid_list[cv->counter -1] = 0; //remove that process from list
		cv->counter-=1;
		spin_unlock(&cv->lock);

		spin_lock(mutex); 
		return;
	}
	sigprocmask(SIG_UNBLOCK, &cv->sigmask, NULL);//unblock
	spin_lock(mutex);
	*/
}

int cv_broadcast(struct cv *cv){
	spin_lock(&cv->lock);
	for (int ii = 0; ii < CV_MAXPROC; ii++){
		kill(cv->pid_list[ii], SIGUSR1);
	}
	spin_unlock(&cv->lock);
	/*
	//lock and unlock
	spin_lock(&cv->lock);
	int temp = 0;
	for (int ii = 0; ii < CV_MAXPROC; ii++){
		if (cv->pid_list[ii] > 0){
				kill(cv->pid_list[ii], SIGUSR1);//send kill, wake up
				temp++;
		}
	}
	spin_unlock(&cv->lock);
	return temp;//returns count of sleepers
	*/
}


int cv_signal(struct cv *cv){
	//lock and unlock
	spin_lock(&cv->lock);	
	int temp = 0;
	//kill the most toppest signal ( if exists)
	if (cv->counter > 0){
		kill(cv->pid_list[cv->counter-1], SIGUSR1);
		temp++;
	}
	spin_unlock(&cv->lock);
	return temp; //should only return 1
}



