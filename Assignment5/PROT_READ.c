#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

void signal_handler(int signo){
	fprintf(stderr,"Signal %d received: %s\n", signo, sys_siglist[signo]);
	signal(signo, SIG_DFL);
	raise(signo);
	exit(signo);		
}

int main(int argc, char* argv[]){
	int fd;
	struct stat sb;
	char* addr;

		
	for (int ii = 1; ii < NSIG; ii++){
		signal(ii, signal_handler);
	}

	if (argc < 2){
		fprintf(stderr,"Wrong number of inputs! Need to supply a filename!\n ");
		return -1;
	}
	
	fd = open(argv[1], O_CREAT | O_RDWR);

	if (fd < 0){
		fprintf(stderr,"Error opening file %s: %s", argv[1], strerror(errno));
		return -1;
	}
	
	if (fstat(fd, &sb) < 0){
		fprintf(stderr,"Error accessing filesize stat %s: %s", argv[1], strerror(errno));
		return -1;
	}
	size_t textlength = sb.st_size+1;

	if (textlength ==0){
		fprintf(stderr,"The file has no size\n");
		return -1;
	}		
	
	addr = mmap(NULL, textlength, PROT_READ, MAP_SHARED, fd,0);
	
	if (addr == MAP_FAILED){
		close(fd);
		fprintf(stderr,"Error mmapping to file :%s\n", strerror(errno));
		return -1;
	}
	
	printf("addr[3] == %c\n", addr[3]);
	printf("Writing a \'B\'\n");
	
	//trying to write a letter B to the mmap	
	addr[3] = 'B';
	
	
	if (msync(addr, textlength, MS_SYNC) < 0){
		fprintf(stderr,"Could not sync file to disk\n");
	}
		
	if (munmap(addr, textlength) < 0){
		close(fd);
		fprintf(stderr,"Error unmapping the file\n");
		return -1;
	}

	if (close(fd)< 0){
		fprintf(stderr,"Error closing fd\n",strerror(errno));
		return -1;
	}
	return 0;
}

		
		
	

	
	
	
	
		
