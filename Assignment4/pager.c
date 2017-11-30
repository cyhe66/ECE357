//PAGER 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
	FILE *stream;
	size_t len = 0;
	ssize_t nread; 
	int count =0;
	char *line = NULL;
	int input;	
	while (( nread = getline(&line,&len, stdin)) != -1) {
		printf("%s", line);
		count ++;	
		
		if (count == 23){
			printf("---Press RETURN for more---\n");	
			stream = fopen("/dev/tty", "w+");
			input = fgetc(stream);
			if (input == 81 || input == 113) { //q or Q
				exit(0);//exit out of program
			}
			count = 0;
		}	
	}
	return 0;
}
