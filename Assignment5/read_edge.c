#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <error.h>


void signal_handler(int signo){
	fprintf(stderr, "Signal %d received: %s\n", signo, sys_siglist[signo]);
	signal(signo, SIG_DFL);
	raise(signo);
	exit(signo);
}

int main(int argc, char* argv[]){
	int fd;
	char buf[2000];
	struct stat sb;
	char* addr;
	ssize_t s;
	char result[1];	
	char result2[1];
	srand(time(NULL));
	
	for (int ii = 1; ii < NSIG; ii++){
		signal(ii, signal_handler);
	}
		
	fd = open("6.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd < 0){
		fprintf(stderr, "Error opening file 6.txt");
		return -1;
	}
	
	for (int ii = 0; ii < 2000; ii++){
		buf[ii] = rand() % 26 + 65;
	}
	
	s = write(fd, &buf, 2000);
	if (s < 0){
		fprintf(stderr,"Error writing to file. %s", strerror(errno));
		return -1;
	}
	
	off_t k = lseek(fd, 0, SEEK_SET);
	if (k < 0){
		fprintf(stderr,"Error with lseek: %s", strerror(errno));
		return -1;
	}
	
	addr = mmap(NULL,8192, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	off_t j = lseek(fd, 2050, SEEK_SET);
	if (j < 0){
		fprintf(stderr, "Error using lseek: %s", strerror(errno));
		return -1;
	}

	printf("1 is %c\n",addr[2500]);//prints
	printf("2 is %c\n",addr[6666]);//creates a bus error
	
	if(munmap(addr, 8192)){
		close(fd);
		fprintf(stderr,"Error unmapping the file\n");
		return -1;
	}
	if (close(fd) < 0){
		fprintf(stderr,"Error closing fd\n", strerror(errno));
		return -1;
	}
	
	return 0;
}
//----------------output----------	
//Signal 7 is recieved: Bus error








