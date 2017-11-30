#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int count = 0;

void sig_handler(int signo){
	fprintf(stderr,"Total Matched Words: %d\n", count);
	signal(SIGPIPE, SIG_DFL);
	raise(SIGPIPE);
}
	 
int main(int argc, char* argv[]){
	FILE *stream;
	char *buff[262144];
	size_t len = 0;
	ssize_t nread;
	int linecount = 0;	
	char *line = NULL;
	size_t len2 = 0;
	ssize_t nread2;	
	char *word = NULL;
	signal(SIGPIPE, sig_handler);
	if (argc == 1){
		fprintf(stderr,"Error: no input file specified.\n");
		return -1;
	}

	stream = fopen(argv[1],"r");
	if (stream == NULL){
		fprintf(stderr,"Error, cannot read file %s: %s\n", argv[1], strerror(errno));
		return -1;
	}

	while ((nread = getline(&buff[linecount], &len, stream)) != -1){
		linecount++;
	}
		
	fclose(stream);//close stream				
	
	while (( nread2 = getline(&word, &len2, stdin)) != -1) {
		for ( int ii = 0; ii < linecount + 1; ii ++){
			if (strcmp(word, buff[ii]) == 0){
				printf("%s", word);
				count ++;
				break;
			}
		}
	}

	for (int ii = 0; ii < linecount + 1; ii ++){
		free(buff[ii]);
	}

	printf("Total Matched Words: %d\n", count);
	
	return (0);
}
