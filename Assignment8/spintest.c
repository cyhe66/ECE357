#include <spinlock.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h> //O_RDWR , O_CREAT, O_TRUNC


//open
//write
//mmap
//fork 
//spinlock
//
//
// ./spintest {spawn_no} {iter_no}
//

int main(int argc, char* argv[]){
	int spawn_no, iter_no;
	int fd;
	char buf[13] = "Hello World!\n";
	struct stat sb;
	int addr;
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
	/*
	 * Write phrase 'Hello World!\n' to the new file './spintest'
	 */

	if (fd = open("./spintest.txt",	O_RDWR|O_CREAT|O_TRUNC, 0666) < 0){
		fprintf(stderr, "Error opening file %s: %s\n", "./spintest.txt", strerror(errno));
		exit(1);
	}
	
	if (write(fd, buf, sizeof(char) * 13) < 0){
		fprintf(stderr, "Error writing to file %s: %s\n", "./spintest.txt", strerror(errno));
		exit(1);
	}	
	/*
	 * Get the size of the written file to feed into mmap
	 * create a shared memory region
	 */
	
	if (fstat(fd, &sb) < 0){
		fprintf(stderr, "Error occurred on fstat for file %s: %s", "./spintest.txt", strerror(errno));
		exit(1);
	}
	
	size_t textlength = sb.st_size + 1;
	
	if (addr = mmap(NULL, textlength, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, fd, 0) < 0){
		fprintf(stderr,"Failure to mmap: %s\n", strerror(errno));
		exit(1);
	}
	
	pid_t pid = 0;

	struct account *l;			// int lock, int target_num, pid_t current holder, int access_count
	/* struct specification */
	l-> lock = 0;
	l-> target_num = 0;

	while (spawn_no){
		//does the fork/exec shit
		switch(pid = fork()){
			case -1: 
				fprintf(stderr,"Error forking process: \n", strerror(errno));
				exit(1);

			case 0: // in child
				while(iter_no){
					spin_lock(l);
					//add a delay
					for (int ii = 0; ii < 10; ii++){
						;
					}	
					spin_unlock(l);
				}
				exit(0);

			default:// in parent
				fprintf(stderr,"Forking Child Process %d\n", spawn_no);
		}
		spawn_no--;
	}

/*
	int wstatus = 0;
	while ((pid = wait(&wstatus)) >0){
		fprintf(stderr, "Exit %d\n", (int) pid);
		if (WIFSIGNALED(wstatus)){
			fprintf(stderr,"Killed by signal: %d\n", WTERMSIG(wstatus));
		}
	}
	
*/

}
