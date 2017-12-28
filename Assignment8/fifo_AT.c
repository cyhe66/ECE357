#include "fifo.c"
int main(int argc, char *argv[]){
	if (argc != 3){
		fprintf(stderr,"User supplied wrong number of arguments. Exiting...\n");
		exit(1);
	}	

	int num_writers = atoi(argv[1]);
	int num_items = atoi(argv[2]);
	
	
	fifo *f;
	f = (fifo*) mmap(NULL, sizeof(fifo), PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, -1,0);
	
	if (f == MAP_FAILED){
		fprintf(stderr,"Error occurred during mmap: %s\n", strerror(errno));
		exit(1);
	}
	fifo_init(f);
	fprintf(stderr, "fifo created.\n");


	//writer stuff/////////////////////////////////////////////
	pid_t fk_pid;
	for (int ii = 0; ii < num_writers; ii++){
		fk_pid = fork();
		if (fk_pid == -1){
			fprintf(stderr,"Error occurred forking process %d\n", ii);
			exit(1);
		}
		else if(fk_pid == 0){
			for(int jj = 0; jj < num_items; jj++){
				fifo_wr(f, jj);
			}
			exit(0);
			fprintf(stderr,"Successfully exited fork %d\n", ii);
		}
		fprintf(stderr,"Writer %d completed.\n", ii);
	}	
	fprintf(stderr, "wrote stuff\n");

	//reader stuff////////////////////////////////////////////
	pid_t rder_pid;
	unsigned long temp_var;	
	rder_pid = fork();
	
	if(rder_pid == -1){
		fprintf(stderr,"Error occurred forking reader");
		exit(1);
	}	
	else if (rder_pid == 0){
		for (int ii = 0; ii <num_writers*num_items; ii++){
			temp_var = fifo_rd(f);
			fprintf(stderr,"Reader stream %d completed.\n", ii);
		}
		fprintf(stderr,"All streams done\n");
		exit(0);
	}	
	fprintf(stderr,"read stuff\n");

	//wait stuff/////////////////////////////////////////////
	for (int ii = 0; ii < num_writers +1; ii++){
		wait(0);
	}
	fprintf(stderr,"Done.\n");
	return 0;
}
	//gg
