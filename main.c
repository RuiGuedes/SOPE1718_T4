#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "Options.c"

#define bufSize 1024

int main(int argc, char* argv[]) {

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
		printf("Invalid call function\n");
		return -1;
	}

	if(initOptions(argc,argv)) {
		if(options[2]) {
			currentDirectory = "./";
			pattern = argv[argc - 1];
		}
		else{
			pattern = argv[argc - 2];
			currentDirectory = argv[argc - 1];
		}
	}
	else
		return -1;

	printOptionsState();

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
	
	return 0;
}
