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

int refilecheck( char* filename, char* pathname, struct stat fileStat, dir *OD, int fd, char *testBuf, struct stat testStat, struct Dirent *de, char testPath, char* og_buf)
{
	int mem = 0; 
	int q = open( testPath, O_RDONLY);

	while (!mem)
	{
		int s = read(q, test_buf, N_BUF);
		int t = read(fd,og_buf, N_BUF);
		if (s < 0)
		{	
			fprintf(stderr,"Error reading file %s: %s\n",de->d_name, strerror(errno));
			exit(-1);
		}
		if ( s==0 && t ==0)
		{
			// we have gotten to the end of both files and mem sill is showing that the files are the same
			return (1);
		}
		mem = memcmp( og_buf, test_buf, N_BUF);
	}
	int c = close(q);
	if (mem)
	{	
		//the files are different 
		return(0);

	}
	else 
	{
		return(1);
	}
}	

void file_traverse(char *filename,char *pathname, struct stat fileStat, DIR *OD, int fd, char* og_buf)
{
	struct stat testStat;
	struct dirent *de;
	DIR *newDir;
//this makes no sense to me!!!
//	if (!(OD = opendir(argv[2])))
//	{
//		fprintf(stderr,"Can not open directory %s: %s\n",argv[2], strerror(errno));
//		return;
//	}
	//so now directory is opened for reading
	while ( de = readdir(newDir))
	{
		char testPath[2048] = {0};
		strcpy(testPath, pathname);
		strcat(testPath, "/");
		strcat(testPath, de->d_name);

		int m = stat(testPath, &testStat);
		if (m < 0)
		{
			fprintf(stderr, "Error occurred accessing file information for %s: %s\n", de->d_name, strerror(errno));
			return;
		}
		unsigned short mode = testStat.st_mode;
		if (S_ISREG(mode) || S_ISLNK(mode))
		{
			if (testStat.st_size == fileStat.st_size)
				// the file sizes are the same, check to see if the contents are the same
			{	
				char* test_buf = malloc(sizeof(char) * N_BUF);
			
				int recheck = refilecheck(test_buf, fileStat, testStat, de, testPath); //TODO missing parameters;
				if (recheck)
				{
					// check if it is a hard link or a duplicate of target or symlink
					if(fileStat.st_dev == testStat.st_dev && fileStat.st_ino == testStat.st_ino) // inode match
					{
						//file is a hard link of target
						printf("%s\t\t HARD LINK TO TARGET", testPath);
					}
					else if ( testStat.st_mode == S_IFLNK)
					{
						//file is a SYMLINK
						//check if it is a symlink of original file or a simlynk to a hard link or something else
						//create a buffer, read simlink into the buffer, and then check if it is the same as the path
						char testContents[2048];
						int fo = open(testPath, O_RDONLY, 0666);
						if (fo < 0)
						{
							fprintf(stderr, "Error opening file %s: %s\n",testPath, strerror(errno));
						}
						int fr = read( fo, testContents, sizeof(testContents) -1);
						if (fr < 0)
						{
							fprintf( stderr, "3Error reading file %s: %s\n", testPath, strerror(errno));
						}
						int symlk = memcmp( testContents,pathname, sizeof(testContents)-1);
						if (!symlink)
						{
							printf("%s\t\t SYMLINK RESOLVES TO TARGET", testPath);
						}
						else 
						{
							printf("%s\t\t SYMLINK RESOLVES TO DUPLICATE: %s", testPath, testContents);
						}
						int ccc = close(fo);
					}
					else
					{		//FILE IS A DUPLICATE OF TARGET (NLINK) == 2
							printf("%s\t\t DUPLICATE OF TARGET (NLINK =	%n)\t", testPath, testStat.st_nlink);
					}
				}
			}
		}
		else if ( S_ISDIR(mode)) // is a directory
//int refilecheck( char* filename, char* pathname, struct stat fileStat, dir *OD, int fd, char *testBuf, struct stat testStat, struct Dirent *de, char testPath, char* og_buf)
		{
			file_traverse(filename, pathname,fileStat, OD, fd, testbuf, testStat, *de, testPath, og_buf);
		}
	}
	int cd = closedir(newDir);
	if (cd < 0 )
	{
		fprintf(stderr, "Error occurred closing directory %s: %s", pathname, strerror(errno));
		return;
	}
}


int main(int argc, char** argv)
{
	int fd;
	if (argc != 3)
	{
		printf("Invalid number of arguments: You entered %d argument(s) instead of 2\n", argc-1);
		return(-1);
	}
	
	DIR* OD = opendir(argv[2]);
	if (OD == NULL)
	{
		fprintf(stderr, "Error occurred opening directory %s: %s", argv[2], strerror(errno));
		exit(-1);
	}
	// now the pathname directory is opened		
	struct stat fileStat;
	int n = stat(argv[1], &fileStat);
	if (n <0 )
	{
		fprintf(stderr, "Error occurred accessing file information for %s: %s", argv[1], strerror(errno));
		exit(-1);
	}
	//load contents into a buffer
	char* og_buf = malloc(sizeof(char) * N_BUF);

	// create the path string 
	//char og_path[2048];
//	strcpy(og_path, argv[2]);
//	strcat(og_path, "/");
//	strcat(og_path, argv[1]);
	fd = open(argv[1],O_RDONLY);
	if ( fd < 0)
	{
		fprintf(stderr,"Error Opening initial file %s: %s", argv[1], strerror(errno));
		exit(-1);
	}
		file_traverse(filename, pathname,fileStat, OD, fd, testbuf, testStat, *de, testPath, og_buf);
	int mcd = closedir(OD);
	if ( mcd < 0 )
	{
		fprintf(stderr," Error closing original directory %s: %s", argv[2], strerror(errno));
	}
}

