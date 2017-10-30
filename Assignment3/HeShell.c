#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/resource.h>


int redirect(char *pathname, int old_fd, int flags, mode_t mode){
	int i;

	fprintf(stdout,"A:\n");
	if (i = open(pathname, flags, mode) > 0){	
		fprintf(stdout,"b:\n%d\n",i);
		fprintf(stdout,"file descriptor i: %d", i);
		if (dup2(i, old_fd) <0) {
			fprintf(stderr,"Error using dup2 fd = %d : %s\n", i, strerror(errno));
			return -1;
		}
		if ( close(i) < 0) {
			fprintf(stderr,"Error on close() for %s: %s\n", pathname, strerror(errno));
			return -1;	
		}
		return 0;
	}
	else {
		fprintf(stderr,"Error opening file location %s: %s\n", pathname, strerror(errno));
		return -1;
	}
}

int dothething(char* command_line){
//	struct timeval t1, t2;//
	struct rusage usage;//get resource usage
	char * commands[4096];
	char *temp, *ret, *fn_reIn = NULL, *fn_reOutT = NULL, *fn_reErrT = NULL, *fn_reOutA = NULL,*fn_reErrA = NULL;
	int i = 0;
	int pid,j,k,m, exitStatus, wStatus;

	command_line[strlen (command_line) - 1] = 0;	//set the new line into 0
		
	temp = strtok( command_line, " ");
	
	while (temp != NULL){
		//parse the line and see if there's ls, pwd, or redirection, and set flags to denote that
		if (strstr(temp, "2>>") == temp){
			fn_reErrA = temp + 3;			//remove the "2>>" from the filename string	
		}
		else if (strstr(temp, ">>") == temp){
			fn_reOutA = temp + 2;
		}
		else if (strstr(temp, "2>") == temp){
			fn_reErrT = temp + 2;	
		}
		else if (strstr(temp, ">") == temp){
			fn_reOutT = temp + 1;
		}
		else if (strstr(temp, "<") == temp){
			fn_reIn = temp + 1;	
		}
		else{
			commands[i] = temp; //feeds the rest of the str into commands
			commands[i+1] = NULL;
			printf("commands[%i] is: %s\n",i, commands[i]);
			i++;
		}
		temp = strtok(NULL, " ");
	}
	//implement change directory

	if (!strcmp(commands[0], "cd")){ 
		//cd by itself
		if (i == 1){
			j = chdir(getenv("HOME"));
			if (j <0){
				fprintf(stderr,"Error changing to HOME directory %s: %s\n",getenv("HOME"), strerror(errno));
				return -1;
			}
			else{
				printf("Directory changed to: %s\n",getenv("HOME"));	
				return 0;
			}
		}
		// cd to change directory
		else {
			j = chdir(commands[1]);
			if (j < 0){
				fprintf(stderr,"Error changing directory %s: %s\n", commands[1], strerror(errno));
				return -1;
			}
			else {
				printf("Directory changed to:%s\n", commands[1]); 
				return 0;
			}
		}			
	}

// now fork the commands line and exec it, 
	switch(pid = fork()){
		case -1:
			fprintf(stderr," Error: fork failure to %s: %s", commands[0],strerror(errno));
			return 1;
		case 0:// child
			//input redirection
			if (fn_reIn != NULL){          // the parameters to the redirection formula
				redirect(fn_reIn,0, O_RDONLY, 0666);
			}
			//output redirection
			if (fn_reOutT != NULL){
				redirect(fn_reOutT, 1, O_RDWR|O_TRUNC|O_CREAT, 0666);
			}
			else if (fn_reOutA != NULL){
				redirect(fn_reOutA, 1, O_RDWR|O_APPEND|O_CREAT, 0666);
			}

			//error redirection
			if (fn_reErrT != NULL){
				redirect(fn_reErrT, 2, O_RDWR|O_TRUNC|O_CREAT, 0666);	
			}
			else if (fn_reErrA != NULL){
				redirect(fn_reErrA, 2, O_RDWR|O_APPEND|O_CREAT, 0666);	
			}

			if (execvp (commands[0], commands) <0){
				fprintf(stderr," '%s' has execvp error:%s\n", commands[0], strerror(errno));
				return -1;
			}
	
		default: //parent
			if( wait3(&wStatus, 0, &usage) > 0 ) {
				if (exitStatus == WIFEXITED(wStatus)){//not normal termination
					fprintf(stderr, "Error: Child process did not terminate normally: %s\n", strerror(errno));
				}
			}
			else{
				fprintf(stderr, "Error: Cannot return pid: %s of child:%s:\n",pid, strerror(errno)); 
			}	
			return 0;
	}	
}

int main(int argc, char** argv){
	FILE *fp_in;
	ssize_t bytesRead;	
	size_t byteSize = 2048;
	char *command_line = NULL;
	if(argc == 1){
		fp_in = stdin;
	}	
	else{
		fp_in = fopen(argv[1], "r");
		if (fp_in == NULL){
			fprintf(stderr,"Error opening file %s: %s", argv[1], strerror(errno));
			return(-1);			
		}  
	}
	while((bytesRead = getline(&command_line, &byteSize, fp_in)) > 0){
		if (bytesRead <= 1 || command_line[bytesRead-1] != '\n'){  //emptyline, or a comment 
			printf("wrong.\n");
			continue;
		}
		else{
			dothething(command_line);
		}		
	}
	
	return 0;
}
