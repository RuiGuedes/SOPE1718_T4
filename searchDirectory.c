#include "searchDirectory.h"
#include "searchFile.h"
#include <dirent.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

/*
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
*/

int searchDirectory(const char * directory) {

	DIR *d;
	struct dirent *dir;
	d = opendir(directory);

	//printf("%s\n",directory );

	if (d) {
		
		while ((dir = readdir(d)) != NULL) {

			pid_t pid;

			if(strcmp(dir->d_name,".") && strcmp(dir->d_name,"..")) {
				char newDir[1024];

				strcpy(newDir,directory);
				strcat(newDir,dir->d_name);

				if(checkFileOrDirectory(newDir) == FILE) {
					
					pid = fork();

					if(pid == 0) {
						searchFile(newDir,pattern);
						exit (0);
					}
				}

				strcat(newDir,"/");

				if(checkFileOrDirectory(newDir) == DIRECTORY) {
					
					pid = fork();

					if(pid == 0) {
						searchDirectory(newDir);
						exit (0); 
					}

				}

			}
			waitpid(pid,NULL,0);
			
		}
	}
	else 
		perror ("Couldn't open the directory");

	
	closedir(d);

	return 0;

}