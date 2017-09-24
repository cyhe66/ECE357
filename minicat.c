#include <fcntl.h>		/* Defines 0_XXXX constants */
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv){
	int fd_in = STDIN_FILENO;		/* standard in */
	int fd_out = STDOUT_FILENO;		/* standard out */
	char opt;
	int arg_counter = 1; 			/* non-input argument counter*/
	int N_BUF = 1024;				/* default buffer */

			/* optional buffer argument and outfile location and errors */
	while((opt = getopt( argc, argv, "b:o:")) != -1){
		if (opt == 'b'){
			N_BUF = strtol(optarg, NULL, 10);
			if (errno == ERANGE){
				fprintf(stderr, "Buffer Error: Buffer %s is out of Range\n",optarg);
				exit(-1);
			}
			if (N_BUF < 1){
				fprintf(stderr, "Buffer Error: Buffer must be greater than 0\n");	
				exit(-1);
			}
			arg_counter = arg_counter + 2;
		}
		else if (opt == 'o'){
			fd_out = open(optarg, O_RDWR | O_CREAT | O_TRUNC, 0644); /* readable by everyone, writable by user */
			if (fd_out == -1){
				fprintf(stderr, "Outfile error: %s is unable to be written to due to %s", optarg, strerror(errno));
				exit(-1);
			}
			arg_counter = arg_counter +2;
		}
		else{
			exit (-1);
		}
	}
	// take the input argv string, starting with the argcounter#, loop through all the inputs, and get them onto the outfile. 
	// initialize malloc 
	char* buf = malloc(sizeof(char) * N_BUF);

	int read_write(int fd_in, int fd_out,int N_BUF ){
		int r = read(fd_in, buf, N_BUF);
		while (r){
			if (r < 0){
				fprintf(stderr, "Can not read file %s: %s\n", argv[optind], strerror(errno));

				exit(-1);
			}

			int	w = write(fd_out,buf, r);
			if (w < 0){
				fprintf(stderr, "Error occurred writing to file %s: %s\n", fd_out, strerror(errno));
			}
			if (w != r){
				r -= w;
				write(fd_out, buf + w, r);// try writing again
			}
			r = read(fd_in, buf, N_BUF);
		}
	}

	int forflag = 0; //	
	for (; optind < argc; ++optind){
		forflag = 1;	
		if (strcmp(argv[optind],"-o") == 0 || strcmp(argv[optind],"-b") ==0){
			// -o or -b is found
			//skip next two arguments 
			optind++;
			continue;
		}
		//open files
		if(strcmp(argv[optind],"-") == 0){
			fd_in = STDIN_FILENO;
		} else {
			fd_in = open(argv[optind],O_RDONLY);
			if (fd_in < 0) {
				fprintf(stderr, "Can not open file %s for reading: %s\n",argv[optind], strerror(errno));
				exit(-1);
			}
		}	
		//read and write
		read_write(fd_in, fd_out, N_BUF);	
	}
	//for loop will not run if optind is == or > argc
	// when that is the case there is no infile
	if ( forflag == 0 ) {
		fd_in = STDIN_FILENO;
		read_write(fd_in, fd_out, N_BUF);
	}
}
