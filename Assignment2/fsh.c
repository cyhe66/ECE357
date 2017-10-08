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

int refilecheck()
{
	int mem = memcmp( *og_buf, *test_buf, N_BUF);
	while (!mem)
	{
		// shift the both buff sizes over and do another compare	
		int s = read(de.d_name, test_buf, N_BUF);
		int j = read((og_path, og_buf, N_BUF);
		if (s < 0)
		{	
			fprintf(stderr,"Error reading file %s: %s\n",de.de_name, streerror(errno));
			exit(-1);
		}
		if (j < 0)
		{	
		fprintf(stderr,"Error reading file %s: %s\n",og_path, streerror(errno));
			exit(-1);
		}
		if !(s == j)
		{
			//files are different, dont care about the files
			// set diff true
		return(0);
		}	
		if ( s==0 && j ==0)
		{
			// we have gotten to the end of both files
			// and mem still is showing that the files are the same
			// set diff boolean FALSE
			return (1);
		}
		mem = memcmp( *og_buf, *test_buf, N_BUF);
	}
	if (mem)
	{	
		//the files are different 
		//set diff TRUE
		return(0);
	}
}	



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
		char testPath[2048];
		strcpy(testPath, pathname);
		strcpy(testPath, "/");
		strcpy(testPath, de.d_name);

		int m = stat(testPath, testStat);
		if (m < 0)
		{
			printf(stderr, "Error occurred accessing file information for %s: %s", de.d_name, strerror(errno));
			exit(-1);
		}
		if (testStat.st_mode == S_IFREG || S_IFLNK) // only check for regular files and symlinks
		{
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
	/////
/////////////////////////
			int recheck = refilecheck(); //TODO missing parameters;
			if (recheck)
			{
				// check if it is a hard link or a duplicate of target or symlink
				if (testStat.st_nlink == 2);
					{		//FILE IS A DUPLICATE OF TARGET (NLINK) == 2
				
					}
				else if ( testStat.st_mode == S_IFLINK){
					{
						//file is a SYMLINK
						//check if it is a symlink of original file or a simlynk to a hard link or something else
					}
				else
					{
						//file is a hard link of target
					}


	while (s) // now compare the contents of both files buffer by buffer to see if there's any difference in data
			{
				int mem = memcmp( *og_buf, *test_buf, N_BUF);
				if (mem)
				{
					// there is a difference in the initial 4096 bytes
					return;
				}
				// check the rest of the file contents to see if the same
					
		
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

	// create the path string 
	char og_path[2048];
	strcpy(og_path, pathname);
	strcpy(og_path, "/");
	strcpy(og_path, filename);

	int r = read(og_path, og_buf, N_BUF); // first 4096 read into og_buffer		

	// stor information about the key file
	
	// call the file_traverse function w/ the current file path

	
}

