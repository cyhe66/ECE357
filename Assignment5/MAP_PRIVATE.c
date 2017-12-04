#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void signal_handler(int signo){
	fprintf(stderr,"Signal %d received: %s\n", signo, sys_siglist[signo]);
	signal(signo, SIG_DFL);
	raise(signo);
	exit(signo);
}	
	
int main( int argc, char* argv[]){
	int fd;
	struct stat sb;
	char * addr;
	char buf[1];	
	for (int ii = 1; ii < NSIG; ii++){
		signal(ii, signal_handler);
	}
	
	if (argc < 2){	
		fprintf(stderr,"Need to provide a filename!\n");
		return -1;
	}	
	
	fd = open(argv[1], O_CREAT | O_RDWR);

	if (fd < 0){
		fprintf(stderr, "Error opening  %s: %s", argv[1], strerror(errno));
		return -1;
	}

	if (fstat(fd, &sb) < 0){
		fprintf(stderr,"Error accessing filesize stat %s: %s", argv[1], strerror(errno));
		return -1;
	}
	size_t textlength = sb.st_size+1; //plus the null character
 
	if (textlength ==0){
		 fprintf(stderr,"The file has no size\n");
		 return -1;
	}
 
    addr = mmap(NULL, textlength, PROT_READ | PROT_WRITE, MAP_PRIVATE,fd,0);
 
    if (addr == MAP_FAILED){
        close(fd);
        fprintf(stderr,"Error mmapping to file :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
    }
	
	addr[3] = 'B';
	
	if (lseek(fd, 3, SEEK_SET) < 0){
		fprintf(stderr,"Error setting the file descriptor: %s", strerror(errno));
		return -1;
	}
	
	int r = read(fd,buf, 1);
	if (r < 0){
		fprintf(stderr,"Error reading byte: %s", strerror(errno));
		return -1;
	}
	
	if (munmap(addr, textlength) < 0){
		close(fd);
		fprintf(stderr, "Error unmapping the file\n");
		return -1;
	}
	if (close(fd) < 0){	
		fprintf(stderr,"Error closing fd\n", strerror(errno));
		return -1;
	}
	
	if (buf[0] == 'B'){
		return 0;
	}
	else{
		return 1;
	}
}	
 
//echo $? returns 1	
