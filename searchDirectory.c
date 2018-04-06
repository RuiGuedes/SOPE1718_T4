#include "searchDirectory.h"
#include "searchFile.h"
#include <dirent.h> 
#include <stdio.h> 

#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
static int
display_info(const char *fpath, const struct stat *sb,int typeflag, struct FTW *ftwbuf)
{		
	int pid = fork();

	if(pid == 0) {

	//printf("%s :: ", fpath);
		if(typeflag == FTW_F)
			searchDirectory(fpath);
		else if(typeflag == FTW_D)
			searchFile(fpath);

		return -1;
	}
    return 0;           // To tell nftw() to continue
}

*/

int checkFileOrDirectory(const char * directory) {

	int file, dir;
	struct stat status;

	if(stat(directory, &status) < 0)
		return STAT_SYSTEM_CALL_FAIL;

	file = S_ISREG(status.st_mode);
	dir = S_ISDIR(status.st_mode);
	
	if(file && !dir)
		return FILE;
	else if(!file && dir)
		return DIRECTORY;
}

int searchDirectory(const char * directory) {

	/*

	int flags = 0;

	if (nftw(directory, display_info, 20, flags) == -1) {
		perror("nftw");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);

	*/

	DIR *d;
	struct dirent *dir;
	d = opendir(directory);
	
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			
			if(strcmp(dir->d_name,".") == 0)
				continue;
			else if(strcmp(dir->d_name,"..") == 0)
				continue;
			else {
				char newDir[1024];
				strcpy(newDir,directory);
				strcat(newDir,dir->d_name);

				if(checkFileOrDirectory(newDir) == FILE) {
					int pid = fork();

					if(pid == 0){
						searchFile(newDir);
						return 0;
					}
				}
				else if(checkFileOrDirectory(newDir) == DIRECTORY) {
					int pid = fork();

					if(pid == 0) {
						searchDirectory(newDir);
						return 0;
					}
				}
			}
		}
		pid_t wpid;
		int status = 0;
		while((wpid = wait(&status)) > 0);

		closedir(d);
	}
	else
		perror ("Couldn't open the directory");
	

	/*
		
		while( exist content on actualDirectory ) {
	
			type = checkContentType();

			if(type == DIRECTORY) {
				
				pid = fork();

				if(pid == 0) {
					exec( searchDirectory( new directory ) )
				}
	
			}
			else if(type == FILE) {
				
				pid = fork();

				if(pid == 0) {
					exec( searchFile( filename ) )
				}
			}
		}

		waitpid(null);

	*/ 


	return 0;

}