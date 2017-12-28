#include "cv.h"


int main(){
		account useless_lock;
		cv* c;
		c = (struct cv *) mmap (NULL, sizeof (cv), PROT_READ | PROT_WRITE, MAP_SHARED| MAP_ANONYMOUS, -1, 0);
		cv_init(c);
		int pid;
		for (int ii = 0; ii < 2; ii++){
			if ((pid= fork()) < 0){
					fprintf(stderr,"ERROR--> forking fork member #[%d]: %s", ii, strerror(errno));
					return 1;
			}
			if (pid == 0){
				fprintf(stderr, "Successfully forked; putting into wait\n");
				cv_wait(c, &useless_lock);
				exit(0);
			}
		}

		sleep(2);
		fprintf(stderr,"Waking them all up\n"); 
		int wakeup = cv_broadcast(c);
		for (int ii = 0; ii < 2; ii++){
			fprintf(stderr, "waiting for my children to die\n");
			wait(0);
		}
		fprintf(stderr,"wakeup # is = %d\n", wakeup);
		return 0;
}

