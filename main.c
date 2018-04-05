#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Options.h"

#define bufSize 1024

#define FILE 0
#define DIRECTORY 1
#define INVALID_OPTIONS -1
#define INVALID_FUNCTION_CALL -2
#define STAT_SYSTEM_CALL_FAIL -3

const char * pattern;
const char * directory;

int checkFileOrDirectory() {

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


int main(int argc, char* argv[], char* envp[]) {

	/*
		(1) simgrep pattern 
				-> Read from shell 
		
		(2) simgrep <options> pattern
				-> If options dont include -r 
					* (1)
				-> If options include -r 
					* Implies recursivity from the directory in which simgrep is executed
		
		(3) simgrep pattern <file/directory>
				-> If file
					* Execute normally
				-> If directory
					* Fail because doens't accept directories

		(4) simgrep <options> pattern <file/directory>
				-> If directory and option -r isn't include
					* Fail
				-> If directory and option -r is include
					* Implies recursivity from the directory passed by parameter
				-> If file
					* Execute normally
	*/

	if(argc < 2) {
		printf("Usage: simgrep <options> pattern <file/directory>\n");
		return INVALID_FUNCTION_CALL;
	}

	int optionsRead = initOptions(argc,argv);
	int remainVariables = argc - (optionsRead + 1);
	int type;

	if(optionsRead == -1) {
		printf("Invalid options. Available options: -l , -c , -r , -w , -i , -n\n");
		return INVALID_OPTIONS;
	}

	//Get directory
	directory = setCurrentDirectory(argc,argv,remainVariables);
	
	//Get pattern
	pattern = setPattern(argc,argv,remainVariables);
	
	//Check if last variable is file(0) or directory(1)
	type = checkFileOrDirectory();

	//Analyze type value possible errors
	if(type == STAT_SYSTEM_CALL_FAIL) {
		printf("simgrep: %s: No such file or directory\n", directory);
		return STAT_SYSTEM_CALL_FAIL;
	}


	// ------------------------------------------------------------- //


	if(type == FILE)
		printf("FILE\n");
	else if(type == DIRECTORY)
		printf("DIRECTORY\n");

	if(optionsRead == 0)
		printf("No options were passed\n");
	else
		printf("Number of options passed: %d\n", optionsRead);


	printf("currentDirectory %s\n", directory);
	printf("Pattern %s\n", pattern);
	printOptionsState();

	return 0;
}

/*	SAMPLE CODE TO EXECUTE SIMGREP
	
	#include <regex.h>

	FILE *fp;
	char buf[bufSize];
	int lineNumber = 1;
	int    status;
    regex_t    re;

    if (regcomp(&re, pattern, REG_EXTENDED|REG_ICASE) != 0) {
        return(0);      // Report error.
    }


	if( (fp = fopen(currentDirectory, "r")) == NULL) {
		printf("Could not open file\n");
		return 1;
	}

	while (fgets(buf, sizeof(buf), fp) != NULL)
	{	
    	buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
    	
    	status = regexec(&re, buf, (size_t) 0, NULL, 0);

    	if(status == 0) {
    		printf("%d:%s\n",lineNumber, buf);
    	}

    	lineNumber++;
	}

	regfree(&re);
	fclose(fp);
*/
