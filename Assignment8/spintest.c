#include "spinlock.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h> //O_RDWR , O_CREAT, O_TRUNC


int main(int argc, char* argv[]){
	int spawn_no, iter_no;
	/* 
	 *Accept command line arguments for # of spawn and # of iterations
	*/	 
	if (argc != 3){
		fprintf(stderr,"Wrong number of commandline arguments!\n");
		exit(1);
	}
	spawn_no = atoi(argv[1]);
	iter_no = atoi(argv[2]);

	if (spawn_no < 1 || iter_no < 1){
		fprintf(stderr,"Invalid arguments.\n");
		exit(1);
	}
	
	int* addr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0 );
	
	if (addr == MAP_FAILED){
		fprintf(stderr,"Failure to mmap: %s\n", strerror(errno));
		exit(1);
	}
	
	pid_t pid = 0;

	account *l;			
	l = (account *) (addr+ sizeof(account)); 
	l-> lock = 0;
	l->target_num = 0;

	for (int ii = 0 ; ii < spawn_no; ii++){
		switch(pid = fork()){
			case -1: 
				fprintf(stderr,"Error forking process: \n", strerror(errno));
				exit(1);

			case 0: // in child
				spin_lock(l);

				for (int ii = 0; ii < iter_no; ii++)
					l->target_num+=1;

				spin_unlock(l);

				exit(0);

			default:// in parent
				fprintf(stderr,"Forking Child Process %d\n", ii);
		}
	}
	//wait for all the child processes
	for (int ii  = 0; ii < spawn_no; ii++){
		wait(0);
	}
	fprintf(stderr,"Final result: %d\n", l->target_num);

}
