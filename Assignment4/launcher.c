#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <fcntl.h>

pid_t pid1, pid2, pid3;

int main(int argc, char* argv[]){
	int filedes[4];
	pid_t wpid1, wpid2, wpid3;
	int stat;
	struct rusage ru;	
	if (argc > 2){
		fprintf(stderr,"Too many arguments");
		return -1;
	}
	if( pipe(filedes) < 0){
		fprintf(stderr,"Error occurred creating first pipe: %s\n", strerror(errno));
		return -1;
	}
	if(pipe(filedes+2) < 0){
		fprintf(stderr,"Error occurred creating the second pipe: %s\n", strerror(errno));
		return -1;
	}	
//filedes[0] ->read
//filedes[1] ->write
//filedes[2] ->read
//filedes[3] ->write
	
	pid1 = fork();//fork the first child
	if (pid1 < 0){
		fprintf(stderr,"You forked up fork! Error: %s\n", strerror(errno));	
		return -1;
	}
	//fork the first child
	else if (pid1 == 0){
		//replace standard output with the output of the pipe
		if((dup2(filedes[1], 1) < 0)){
			fprintf(stderr,"Error occurred with dup: %s\n", strerror(errno));
			return -1;
		}
		close(filedes[0]);
		close(filedes[1]);
		close(filedes[2]);
		close(filedes[3]);
		
		if ((execlp("./wordgen","0", argv[1], (char *) NULL)) < 0){
			fprintf(stderr,"Error occurred during exec of ./wordgen : %s\n", strerror(errno));
	
			return -1;
		}
	}
	else {
		//fork the second child
		pid2 = fork();
		
		if (pid2 < 0){
			fprintf(stderr, "You forked up fork! Error: %s\n", strerror(errno));
			return -1;
		}
		else if (pid2 == 0){
			if (dup2(filedes[0], 0) < 0){
				fprintf(stderr,"Error occurred with dup: %s\n", strerror(errno));
				return -1;
			}
			if (dup2(filedes[3], 1) < 0){
				fprintf(stderr,"Error occurred with dup: %s\n", strerror(errno));
				return -1;
			}
			//close all pipes
			close(filedes[0]);
			close(filedes[1]);
			close(filedes[2]);
			close(filedes[3]);
		
			if((execlp("./wordsearch", "0", "dictionaryBIG", (char*) NULL)) < 0){
				fprintf(stderr,"Error occurred during exec of ./wordsearch: %s\n", strerror(errno));
				return -1;
			}
		}
		else{
			pid3 = fork();
			
			if (pid3 < 0){
				fprintf(stderr, "You forked up fork! Error : %s\n", strerror(errno));
				return -1;
			}
			else if ( pid3 == 0){	
			
				if (dup2(filedes[2], 0) < 0){
					fprintf(stderr,"Error occurred with dup: %s\n", strerror(errno));
					return -1;
				}
				close(filedes[0]);
				close(filedes[1]);
				close(filedes[2]);
				close(filedes[3]);
		
				if(execlp("./pager", (char*) NULL) < 0) {
					fprintf(stderr, "Error occurred during exec of ./pager: %s\n", strerror(errno));
					return -1;
				}
			}
		}
	}
	close(filedes[0]);
	close(filedes[1]);
	close(filedes[2]);
	close(filedes[3]);
//print out the exit status	
	wpid1 =	wait3(&stat, 0, &ru);
	fprintf(stderr,"Process %d exited with %d\n", wpid1, stat);
	wpid2 = wait3(&stat, 0, &ru);
	fprintf(stderr,"Process %d exited with %d\n", wpid2, stat);
	wpid3 = wait3(&stat, 0, &ru);
	fprintf(stderr,"Process %d exited with %d\n", wpid3, stat);
	
}

				

