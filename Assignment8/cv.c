#include "cv.h"

//globals
volatile sig_atomic_t usr_interrupt = 0;
sigset_t mask, oldmask;
struct sigaction s_act;

void handler(int signo){
	usr_interrupt = 1;	
}

void cv_init(struct cv *cv){
	cv->counter = 0;
	
	for (int ii = 0; ii < CV_MAXPROC; ii++){
		cv->pid_list[ii] = 0;
	}

	spin_unlock(&cv->lock);	

	s_act.sa_handler = handler;
	sigaction(SIGUSR1, &s_act, NULL);
	sigemptyset(&mask);//mask all the defined signals //create the mask
	sigaddset(&mask, SIGUSR1); //exclude the SIGUSR1 signal
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
	usr_interrupt = 0;
	cv->lock = *mutex;
	cv->pid_list[cv->counter] = (int) getpid();
	cv->counter++;
	sigprocmask(SIG_BLOCK, &mask, &oldmask); // block all signals in sigmask 
	spin_unlock(mutex);
	spin_unlock(&cv->lock);

	while(!usr_interrupt){
		sigsuspend(&oldmask);
	}
	spin_lock(mutex);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);
}

int cv_broadcast(struct cv *cv){
	//lock and unlock
	spin_lock(&cv->lock);
	int temp = 0;
	for (int ii = 0; ii < cv->counter; ii++){
		if (cv->pid_list[ii] > 0){
				kill(cv->pid_list[ii], SIGUSR1);//send kill, wake up
				temp++;
		}
	}
	cv->counter = 0;
	spin_unlock(&cv->lock);
	return temp;//returns count of sleepers
}


int cv_signal(struct cv *cv){
	//lock and unlock
	spin_lock(&cv->lock);	
	int temp = 0;
	//kill the most toppest signal ( if exists)
	if (cv->counter > 0){
		kill(cv->pid_list[cv->counter-1], SIGUSR1);
		temp++;
		cv->counter--;
	}
	spin_unlock(&cv->lock);
	return temp; //should only return 1
}



