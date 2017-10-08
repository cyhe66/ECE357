#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define N_BUF 4096


int file_traverse( char filename, char pathname)
{
	struct stat testStat;
	struct dirent *de;
	DIR *dirp;
	if (!(dirp = opendir(pathname))
	{
		fprintf(stderr,"Can not open directory %s:%s", pathname, strerror(errno));
		exit (-1);
	}
	//so now i opened the dir, must read the directory
	while ( de = readdir(dirp))
	{
		int m = stat(de.d_name, testStat);
		if (m < 0)
		{
			printf(stderr, "Error occurred accessing file information for %s: %s", de.d_name, strerror(errno));
			exit(-1);
		}
		test_size =testStat.st_size;
		if (testStat.st_size == fileStat.st_size)
		{	
		// the file sizes are the same, look into the file and see if the contents are the same
		char* test_buf = malloc(sizeof(char) * N_BUF);
		int s = read(de.d_name, test_buf, N_BUF);
		if (s<0)
		{
			fprintf(stderr," Error occurred reading file %s: %s", de.d_name, strerror(errno));
			exit(-1);
		}
		while (s)
			{
			
		
}

int main(int argc, char** argv)
{

//parse the input of the terminal as the file to be checked
//find the file in question and record all the information about it
//then traverse the rest of the files in the directory going down every path 
// returning hard links and soft links and similar files and their sizez
//use opendir closedir readdir
//also be able to read binart files...(strncmp and memcmp difference?)
//what ways to disqualify an inode as a potential match?
////do a size comparison
////
	if (argc != 3)
	{
		printf("Invalid number of arguments: You entered %d argument(s) instead of 2\n", argc-1);
		return(-1);
	}
	char filename[1024]; 
	strcpy(filename, argv[1]);
	char pathname[1024];
	strcpy(pathname, argv[2]);
	
	char OD = opendir(pathname);
	if (OD == NULL)
	{
		fprintf(stderr, "Error occurred opening directory %s: %s", pathname, strerror(errno));
		exit(-1);
	}
	// now the pathname directory is opened		
	struct stat fileStat;
	int n = stat(filename, &fileStat);
	if (n <0 )
	{
		printf(stderr, "Error occurred accessing file information for %s: %s", filename, strerror(errno));
		exit(-1);
	}
	//load contents into a buffer
	char* og_buf = malloc(sizeof(char) * N_BUF);
	int r = read( filename, og_buf, N_BUF); // first 4096 read into og_buffer		

	// stor information about the key file
	
	// call the file_traverse function w/ the current file path

	
}

