#include "searchDirectory.h"
#include "searchFile.h"
#include <dirent.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


int searchDirectory(const char * directory) {

	DIR *d;
	struct dirent *dir;
	d = opendir(directory);	


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
						signal(SIGINT, SIG_IGN);
						searchFile(newDir);
						exit (0);
					}
				}

				strcat(newDir,"/");

				if(checkFileOrDirectory(newDir) == DIRECTORY) {
					
					pid = fork();

					if(pid == 0) {
						signal(SIGINT, SIG_IGN);
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