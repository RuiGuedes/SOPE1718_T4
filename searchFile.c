#include "searchFile.h"
#include "Options.h"

int PRINTED = 0;

int searchFile(const char * fileDirectory, const char * pattern) {

	int lineNumber = 1;
	int status;
	char buf[bufSize];
	regex_t re;

	if(checkICASE()) {
		if (regcomp(&re, pattern, REG_EXTENDED|REG_ICASE) != 0) {
        	return(0);      // Report error.
        }
    }
    else {
    	if (regcomp(&re, pattern, REG_EXTENDED) != 0) {
        	return(0);      // Report error.
        }
    }

    if(checkCompleteWord())
    	searchFileWordByWord(fileDirectory,pattern,re);


   // otherFunctionTest(fileDirectory,pattern,re);

    regfree(&re);

    return 0;

}

int otherFunctionTest(const char * fileDirectory, const char * pattern, regex_t re) {

	int lineNumber = 1;
	int status;
	char buf[bufSize];

	FILE *file = fopen(fileDirectory, "r");
	if(file == NULL) {
		printf("Could not open file\n");
		return 1;
	}

	while (fgets(buf, sizeof(buf), file) != NULL)
	{	
    	buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
    	
    	status = regexec(&re, buf, (size_t) 0, NULL, 0);

    	if(status == 0) {
    		
    		const char s[2] = " ";
    		char *token;

  			 /* get the first token */
    		token = strtok(buf, s);

   			/* walk through other tokens */
    		while( token != NULL ) {
    			
    			if(strcasecmp(token,pattern) == 0) {
    				printf( BOLDRED "%s " DEFAULT, token);
    			}
    			else {
    				printf( "%s ", token);
    			}
    			
    			token = strtok(NULL, s);
    		}
    		printf("\n");
    		//printf("%d:%s\n",lineNumber, buf);
    	}

    	lineNumber++;
    }


    fclose(file);

    return 0;

}

int searchFileWordByWord(const char * fileDirectory, const char * pattern, regex_t re) {

	char buf[bufSize];
	int lineNumber = 1;
	int numberOfLines = 0;
	int status;

	FILE *file = fopen(fileDirectory, "r");
	if(file == NULL) {
		printf("Could not open file\n");
		return FILE_OPEN_ERROR;
	}

	while (fgets(buf, sizeof(buf), file) != NULL) {	

		buf[strlen(buf) - 1] = '\0';

		status = regexec(&re, buf, (size_t) 0, NULL, 0);

		if(status == 0) {

			numberOfLines++;

			if(checkFileName()) {
				printf(MAGENTA "%s\n", fileDirectory);
				break;
			}

			if(checkRecursivity() && !PRINTED) {
				PRINTED = 1;
				printf(MAGENTA "%s", fileDirectory);
				printf(CYAN ":");
			}

			if(!checkLines())
			{
				const char s[2] = " ";
				char * token;
				token = strtok(buf, s);

				while(token != NULL) {

					if(strcasecmp(token,pattern) == 0) {
						printf(BOLDRED "%s ", token);
					}
					else {
						printf(DEFAULT "%s ", token);
					}

					token = strtok(NULL, s);
				}

				printf("\n");
			}
		}

		lineNumber++;
	}

	if(checkLines()) {
		printf(DEFAULT "%d\n",numberOfLines);
	}

	fclose(file);

	return SUCESS;
}