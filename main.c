#include "Options.h"
#include "searchDirectory.h"

#define bufSize 1024

#define INVALID_OPTIONS -1
#define INVALID_FUNCTION_CALL -2

const char * pattern;

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
		printf("Read from shell\n");
	else if(checkRecursivity() && (lastVariabletype == DIRECTORY)) {
		printf("Implies recursivity\n" );
		searchDirectory(executionDirectory);
	}
	else if(lastVariabletype == FILE)
		printf("Execute normally\n" );
	else if((lastVariabletype == DIRECTORY) && !checkRecursivity())
		printf("simgrep: %s: Is a directory\n",executionDirectory);
	
	// ------------------------------------------------------------- //

	/*
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
	*/
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
