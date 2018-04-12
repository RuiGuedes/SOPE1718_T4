#include "searchDirectory.h"
#include "searchFile.h"
#include <dirent.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


int searchDirectory(const char * directory, int argc , char* argv[], int remainVariables) {

	DIR *d;
	struct dirent *dir;
	d = opendir(directory);	

	if(remainVariables == 1)
		argc += 2;
	else 
		argc ++;

	char* argNew[argc];
	const char * sv = "/home/rui/SOPE/SOPE1718_T4T1";

	if(remainVariables == 1) {
		for(int i = 0; i < (argc - 1); i++)
			argNew[i] = argv[i];
	}
	else {
		for(int i = 0; i < argc; i++) {
			argNew[i] = argv[i];
		}
	}

	argNew[argc - 1] = NULL;

	if (d) {

		while ((dir = readdir(d)) != NULL) {

			pid_t pid;

			if(strcmp(dir->d_name,".") && strcmp(dir->d_name,"..")) {
				char newDir[1024];

				strcpy(newDir,directory);
				strcat(newDir,dir->d_name);

				if(strcmp(newDir,executionDirectory) == 0)
					continue;

				if(checkFileOrDirectory(newDir) == FILE) {
					pid = fork();

					if(pid > 0) {
						waitpid(pid,NULL,0);
					}
					else if(pid == 0) {
						execvp("./simgrep",argNew);
						exit (0);
					}
				}

				strcat(newDir,"/");
				argNew[argc - 2] = newDir;

				if(checkFileOrDirectory(newDir) == DIRECTORY) {

					pid = fork();

					if(pid > 0) {
						waitpid(pid,NULL,0);
					}
					else if(pid == 0) {
						execvp("./simgrep",argNew);
						printf("%s\n", argNew[argc - 2]);
						exit (0); 
					}

				}

			}

		}
	}
	else 
		perror ("Couldn't open the directory");


	closedir(d);

	return 0;

}