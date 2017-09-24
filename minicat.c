#include <fcntl.h>		/* Defines 0_XXXX constants */
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int read_write(int fd_in, int fd_out,int N_BUF, void* buf, int argc, char** argv){		//read & write function
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

int main(int argc, char** argv){
	int fd_in = STDIN_FILENO;		/* standard in */
	int fd_out = STDOUT_FILENO;		/* standard out */
	char opt;
	int arg_counter = 1; 			/* non-input argument counter*/
	int N_BUF = 1024;				/* default buffer */

	while((opt = getopt( argc, argv, "b:o:")) != -1){  // find the optional arguments to the command
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
	char* buf = malloc(sizeof(char) * N_BUF);

	int forflag = 0;			// flag to determine whether or not the for loop was executed	
	for (; optind < argc; ++optind){
		forflag = 1;	
		if (strcmp(argv[optind],"-o") == 0 || strcmp(argv[optind],"-b") ==0){	// look only at possible input files
			optind++;
			continue;
		}
		if(strcmp(argv[optind],"-") == 0){		// open files 
			fd_in = STDIN_FILENO;
		} else {
			fd_in = open(argv[optind],O_RDONLY);
			if (fd_in < 0) {
				fprintf(stderr, "Can not open file %s for reading: %s\n",argv[optind], strerror(errno));
				exit(-1);
			}
		}	
		//read and write
		read_write(fd_in, fd_out, N_BUF, buf, argc, argv);		// call read_write function	
	}
	if ( forflag == 0 ) {						// if for loop isn't run, aka no infiles, read through standard input
		fd_in = STDIN_FILENO;
		read_write(fd_in, fd_out, N_BUF, buf, argc, argv);
	}
}
