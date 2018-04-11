#include "searchDirectory.h"
#include "searchFile.h"
#include "readFromConsole.h"

#define INVALID_OPTIONS -1
#define INVALID_FUNCTION_CALL -2


void sigintHandler(int signum) {
 
 	char answer;
 	printf(" - Are you sure you want to terminate the program? (Y/N) ");
 	scanf("%c", &answer);

 	if(answer == 'Y')
 		exit(0);

}


int main(int argc, char* argv[], char* envp[]) {

	/*
		(1) simgrep pattern 
				-> Read from shell  [CHECK]
		
		(2) simgrep <options> pattern
				-> If options dont include -r [CHECK]  
					* (1)
				-> If options include -r [CHECK]
					* Implies recursivity from the directory in which simgrep is executed
		
		(3) simgrep pattern <file/directory>
				-> If file [CHECK]
					* Execute normally
				-> If directory
					* Fail because doens't accept directories

		(4) simgrep <options> pattern <file/directory>
				-> If directory and option -r isn't include
					* Fail
				-> If directory and option -r is include [CHECK]
					* Implies recursivity from the directory passed by parameter
				-> If file [CHECK]
					* Execute normally
	*/

	if(argc < 2) {
		printf("Usage: grep [OPTION]... PATTERN [FILE/DIRECTORY]...\n");
		return INVALID_FUNCTION_CALL;
	}

	struct sigaction action;
	action.sa_handler = sigintHandler;

	if (sigaction(SIGINT, &action, NULL) < 0) {
		perror ("Sigaction: ");
		return 1;
	}


	int optionsRead = initOptions(argc,argv);
	int remainVariables = argc - (optionsRead + 1);
	int lastVariabletype;

	if(optionsRead == INVALID_OPTIONS) {
		printf("Invalid options. Available options: -l , -c , -r , -w , -i , -n\n");
		return INVALID_OPTIONS;
	}
	else if(!remainVariables) {
		printf("Usage: simgrep [OPTION]... PATTERN [FILE/DIRECTORY]...\n");
		return INVALID_FUNCTION_CALL;
	}

	//Get executionDirectory
	executionDirectory = setCurrentDirectory(argc,argv,remainVariables);
	
	//Get pattern
	pattern = setPattern(argc,argv,remainVariables);

	//Check if last variable is file(0) or directory(1)
	lastVariabletype = checkFileOrDirectory(executionDirectory);

	//Analyze type value possible errors
	if(lastVariabletype == STAT_SYSTEM_CALL_FAIL) {
		printf("simgrep: %s: No such file or directory\n", executionDirectory);
		return STAT_SYSTEM_CALL_FAIL;
	}

	//Check the needed response to a certain input
	if(((optionsRead == 0) || !checkRecursivity()) && (remainVariables == 1)) 
		readFromConsole(pattern);
	else if(checkRecursivity() && (lastVariabletype == DIRECTORY)) {
		searchDirectory(executionDirectory);
	}
	else if(lastVariabletype == FILE)
		searchFile(executionDirectory);
	else if((lastVariabletype == DIRECTORY) && !checkRecursivity())
		printf("simgrep: %s: Is a directory\n",executionDirectory);


	return SUCESS;
}
