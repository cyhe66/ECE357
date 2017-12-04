#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>	
#include <string.h>
	
int main(int argc, char* argv[]){
	int fd;
	char buf[6000];
	ssize_t s;
	struct stat sb;
	struct stat sd;
	char* addr;

	srand(time(NULL));

	fd = open("4.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd < 0){
		fprintf(stderr,"Error opening file 4.txt");
		return -1;
	}
	
	for(int ii = 0; ii < 6000; ii++){
		buf[ii] = rand() % 26 + 65;		
	}
	//garbage is generated
	s = write(fd, &buf, 6000);
	if( s < 0 ){
		fprintf(stderr,"Error writing to file. %s", strerror(errno));
		return -1;
	}	
	off_t k = lseek(fd, 0, SEEK_SET);
	if (k < 0){
		fprintf(stderr,"Error with lseek: %s", strerror(errno));
		return -1;
	}
	
	if (fstat(fd, &sb) < 0){
		fprintf(stderr,"Error accessing filesize stat: %s", strerror(errno));
		return -1;	
	}
	
	size_t before = sb.st_size + 1;

	addr = mmap(NULL, 6000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(addr == MAP_FAILED){
		close(fd);
		fprintf(stderr,"Error mmapping to file: %s\n", strerror(errno));
		return -1;
	}
	
	addr[6001] = 'g';
	
	if (fstat(fd, &sd)){
		fprintf(stderr,"Error accessing filesize stat: %s", strerror(errno));
		return -1;
	}
	size_t after = sd.st_size + 1;
	
	if (munmap(addr, 6000) < 0){
		close(fd);
		fprintf(stderr,"Error unmapping the file\n");
		return -1;
	}
	if (close(fd) < 0){
		fprintf(stderr,"Error closing fd\n", strerror(errno));
		return -1;
	}

	if (before == after){
		return 1;
	}	
	else{
		return 0;
	}
}
