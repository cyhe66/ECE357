#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc,char *argv[]){
	//no argument is provided
	srand(time(NULL));//seed 
	if (argc == 1){	
		while (1){
			char word[16];
			int wlen = rand() % 10 + 3;
			for (int ii = 0; ii < wlen; ii++){
				word[ii] = rand() % 26 + 65;	
			}
			printf("%s\n", word);
			memset(word, 0,16);
		}	
	}
	//generate some random words	
	else if (argc > 1){
		int lim = atoi(argv[1]);
		for (int ii = 0; ii < lim; ii++){
			char word[16];
			int wlen = rand() % 10 + 3;
			for (int ii = 0; ii < wlen; ii++){
				word[ii] = rand() % 26 + 65;
			}
			printf("%s\n", word);
			memset(word, 0,16);
		}
	}
	return 0;
}





