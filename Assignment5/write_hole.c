#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <error.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char* argv[]){
	int fd;
	char buf[6000];
	char result[1];
	ssize_t s;
	struct stat sb;
	char* addr;
	
	srand(time(NULL));
	
	fd = open("5.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd < 0){
		fprintf(stderr,"Error opening file 5.txt");
		return -1;
	}
	
	for(int ii = 0; ii < 6000; ii++){
		buf[ii] = rand() % 26 + 65;
	}
	
	s = write(fd, &buf, 6000);
	if (s < 0){
		fprintf(stderr,"Error writing to file. %s", strerror(errno));
		return -1;
	}
	off_t k = lseek(fd, 0, SEEK_SET);


	if(k< 0){
		fprintf(stderr,"Error with lseek: %s", strerror(errno));
		return -1;
	}
	
	if ( fstat(fd, &sb) < 0){
		fprintf(stderr,"Error accessing filesize stat: %s", strerror(errno));
		return -1;
	}
	size_t before = sb.st_size +1;

	addr = mmap(NULL, 6000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(addr == MAP_FAILED){
		close(fd);
		fprintf(stderr,"Error mmapping filesize statL %s", strerror(errno));
		return -1;
	}
	
	addr[6000] = 'g';
	
	off_t j = lseek(fd, 6016, SEEK_SET);
	if ( j < 0){	
		fprintf(stderr,"Error using lseek: %s", strerror(errno));
		return -1;
	}
	char blah = 'b';

	ssize_t as = write(fd, &blah, 1);
	if (as < 0){
		fprintf(stderr," Error writing to file: %s", strerror(errno));
		return -1;
	}
	
	off_t q = lseek(fd, 6000, SEEK_SET);
	
	ssize_t m = read(fd, &result, 1);
	if (m < 0){
		fprintf(stderr,"Error reading file: %s",strerror(errno));
		return -1;
	}

	if(munmap(addr, 6000) < 0){
		close(fd);
		fprintf(stderr,"Error unmapping the file\n");
		return -1;
	}
	if(close(fd)<0){
		fprintf(stderr,"Error closing fd\n", strerror(errno));	
		return -1;
	}

	if (result[0] == 'g'){
		return 0;
	}	
	else{
		return 1;
	}

}

	
