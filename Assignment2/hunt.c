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

void printpermissions(int permissioncheck)
{
	if (permissioncheck ==1)
	{
		printf("Readable by others\n");
	}
	else 
	{
		printf("Not Readable by others \n");
	}	
}

/*
int refilecheck(char* filename, char* pathname, char* testPath, struct stat fileStat, struct stat testStat, DIR *OD, DIR *ND, int ffile_fd)
{
	int mem,i,j, k;
	//create some buffers for reading files into
	char* test_buf = malloc(sizeof(char) * N_BUF);
	char* og_buf = malloc(sizeof(char) * N_BUF);
	

	i = open(testPath, O_RDONLY);
	if (i<0)
	{
		fprintf(stderr,"Error opening file %s: %s",testPath, strerror(errno));
		return (-1);
	}

	// exit conditions
	// 1) mem != 0, memcmp returned a difference in the files
	// 2) read read to the end of the file, and thus break breaks out of the file
	while (!mem ) //if mem is zero, the two files match so far
	{
		if (j == 0 && k ==0)//both # of bytes for both files are 0, both files have reached the end
		{	//mem must still be 0 for this to execute	
			break;
		}
		j = read(i,test_buf, N_BUF);// read testPath file into the test_buf
		k = read(ffile_fd,og_buf, N_BUF);// read argv[1] into og_buf
		if (j < 0)
		{
			fprintf(stderr,"Error reading file %s: %s", testPath, strerror(errno));
			exit(-1);
		}
		if (k < 0)
		{
			fprintf(stderr, "Error reading file %s: %s", testPath, strerror(errno));
			exit(-1);
		}
		mem = memcmp(test_buf, og_buf, N_BUF);
	}
	if (!mem)
	{
		return 2;//the files' contents are the same
	}	
	else
	{// mem != 0, and the files are not the same. move onto the next file
		return 1;
	}
	close(j);
}
*/
int file_traverse(char* filename, char* pathname, char* og_Path,int permissioncheck)
{
	struct stat fileStat,testStat, dirStat;
	struct dirent *de;
	DIR *ND, *OD;//newdir and olddir
	int i,j, rfchk;
	char testPath [2048] = {0};
	char* in_sym = malloc(sizeof(char) *N_BUF);

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "Error opening Initial file %s: %s", filename, strerror(errno));
	}
	
	i = stat(filename, &fileStat);
	if (i < 0)
	{
		fprintf(stderr,"Error occurred accessing file information for %s: %s", filename, strerror(errno));
		exit(-1);
	}
	
	OD = opendir(pathname);	
	if ( OD == NULL)
	{
		fprintf(stderr, "Error occurred opening initial directory %s: %s", pathname, strerror(errno));
	}
	

	if ( stat(pathname, &dirStat) >= 0 && dirStat.st_mode & S_IROTH)
	{
		permissioncheck = 1;
	}
	else 
	{
		permissioncheck = 0;
	}

	while (de = readdir(ND))
	{	
		//create the new directory string
		strcpy(testPath, pathname);
		strcat(testPath, "/");
		strcat(testPath, de->d_name);
		
		if (S_ISDIR(testStat.st_mode)) // is a directory 
		{
			ND = opendir(testPath);
			if (ND == NULL)
			{
				fprintf(stderr,"Error opening directory %s: %s", testPath, strerror(errno));
				return(-1);
			} 
			file_traverse(filename, pathname, fileStat, OD, ffile_fd, og_Path, permissioncheck);
			i = closedir(ND);
			if (i<0)
			{
				fprintf(stderr,"Error closing directory %s: %s", testPath, strerror(errno));
			}
		}
		else if (S_ISREG(testStat.st_mode)) // is a normal file
		{
			j = stat(testPath, &testStat);
			if (j < 0)
			{
				fprintf(stderr,"Error occurred accessing file information for %s: %s", filename, strerror(errno));
				printpermissions(permissioncheck);
				exit(-1);
			}
			if (testStat.st_size == fileStat.st_size) // either hard link or duplicate
			{
				rfchk = refilecheck(filename, pathname, testPath, testStat, fileStat, OD, ND, ffile_fd);  //run recheck function
				if 	(rfchk == 2 && testStat.st_dev == fileStat.st_dev && fileStat.st_ino == testStat.st_ino) 
				{
					printf("%s\tHard Link to target\t", testPath);
					printpermissions(permissioncheck);
					break;
				}
				else if (rfchk ==2)
				{
					printf("%s\t Duplicate of Target (Nlink = %n)",testPath, testStat.st_nlink);
					printpermissions(permissioncheck);
					break;
				}
			}
		}
		else if (S_ISLNK(testStat.st_mode)) // is a symlink 
		{
			int op = open(testPath, O_RDONLY);
			int rd = read(op, in_sym, N_BUF);	
		
			if (in_sym == og_Path)//contents of sym is the address of the original file
			{	
				printf("%s\tSymlink resolves to target\t", pathname);
				printpermissions(permissioncheck);
				break;
			}
			else
			{
				printf("%s\tSymlink resolves to copy of target\t (nlink = %n)\t", pathname, testStat.st_nlink); printpermissions(permissioncheck);
				break;
			}
		}
	}// close while loop todo free some stuff	, return 0?
}			

int main(int argc, char** argv)
{
	int fd, i, j;
	struct stat fileStat;
	DIR* OD;
	char og_Path [2048] = {0};

	if (argc != 3)
	{
		printf("Invalid number of arguments: You entered %d argument(s) instead of 2\n",argc-1);
		exit(-1);
	}
/*	
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "Error opening Initial file %s: %s", argv[1], strerror(errno));
	}
	
	i = stat(argv[1], &fileStat);
	if (i < 0)
	{
		fprintf(stderr,"Error occurred accessing file information for %s: %s", argv[1], strerror(errno));
		exit(-1);
	}
	
	OD = opendir(argv[2]);	
	if ( OD == NULL)
	{
		fprintf(stderr, "Error occurred opening initial directory %s: %s", argv[2], strerror(errno));
	}
*/
	strcpy(og_Path, argv[2]);
	strcat(og_Path, "/");
	strcat(og_Path, argv[1]);
	
	j = file_traverse(argv[1], argv[2], og_Path, 1);
	if (j <0)
	{
		fprintf(stderr,"Error occurred reading initial file %s: %s", argv[1], strerror(errno));
	}
//	close (fd);
//	closedir(OD);
//	printf("%s\n", og_Path);
	return(0);
}
