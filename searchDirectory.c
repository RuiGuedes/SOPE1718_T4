#include "searchDirectory.h"
#include <dirent.h> 
#include <stdio.h> 

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
			
				if(checkFileOrDirectory(newDir) == FILE)
					printf("FILE:  %s\n", newDir);
				else if(checkFileOrDirectory(newDir) == DIRECTORY)
					printf("DIRECTORY:  %s\n", newDir);
			}
		}
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