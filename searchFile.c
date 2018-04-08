#include "searchFile.h"
#include "Options.h"


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
    else
    	searchFileWord(fileDirectory,pattern,re);

    regfree(&re);

    return 0;

}

int searchFileWord(const char * fileDirectory, const char * pattern, regex_t re) {

	char buf[bufSize];
	int lineNumber = 1;
	int numberOfLines = 0;
	int status, info;

	FILE *file = fopen(fileDirectory, "r");
	if(file == NULL) {
		printf("Could not open file\n");
		return FILE_OPEN_ERROR;
	}

	info = checkPatternExistence(fileDirectory,pattern,re);

	while (fgets(buf, sizeof(buf), file) != NULL) {	

		buf[strlen(buf) - 1] = '\0';
		char * word;

		const char s[2] = " ";
		char * token;
		token = strtok(buf, s);

		while(token != NULL) {

			if(checkICASE()) 
			{
				if(strcasecmp(token,pattern) == 0) {
					printf(BOLDRED "%s ", token);
				}
				else 
				{
					if((word = strcasestr(token, pattern)) != NULL) {
						
						char * miniToken;
						miniToken = strtok(token, word);

						int printed = 1;
						while(miniToken != NULL) {

							printf(DEFAULT "%s", miniToken);
							
							miniToken = strtok(NULL, word);
						}
						printf(" ");
					}	
					else 
						printf(DEFAULT "%s ", token);
				}
			}
			else {
				if(strcmp(token,pattern)) {
					printf(BOLDRED "%s ", token);
				}
				else {

					if((word = strstr(token,pattern)) != NULL) {

					}
					else {
						printf(DEFAULT "%s ", token);
					}
				} 

			}

			token = strtok(NULL, s);
		}

		printf("\n");
		lineNumber++;
	}

	if(checkLines()) {
		printf(DEFAULT "%d\n",numberOfLines);
	}

	fclose(file);

	return SUCESS;

}

int searchFileWordByWord(const char * fileDirectory, const char * pattern, regex_t re) {

	char buf[bufSize];
	int lineNumber = 1;
	int numberOfLines = 0;
	int status, info;

	FILE *file = fopen(fileDirectory, "r");
	if(file == NULL) {
		printf("Could not open file\n");
		return FILE_OPEN_ERROR;
	}

	info = checkPatternExistence(fileDirectory,pattern,re);

	while (fgets(buf, sizeof(buf), file) != NULL) {	

		buf[strlen(buf) - 1] = '\0';

		status = regexec(&re, buf, (size_t) 0, NULL, 0);

		if(status == 0) {

			numberOfLines++;

			if(checkFileName()) {
				printf(MAGENTA "%s\n", fileDirectory);
				break;
			}

			if(!checkLines())
			{	
				if(checkRecursivity() && (info == EXISTS)) {
					printf(MAGENTA "%s", fileDirectory);
					printf(CYAN ":");
				}

				if(checkLineNumber()) {
					printf(GREEN "%d", lineNumber);
					printf(DEFAULT ":");
				}

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

int checkPatternExistence(const char * fileDirectory, const char * pattern, regex_t re) {

	char buf[bufSize];
	int status, state = -1;

	FILE *file = fopen(fileDirectory, "r");
	if(file == NULL) {
		return FILE_OPEN_ERROR;
	}

	while (fgets(buf, sizeof(buf), file) != NULL) {	

		buf[strlen(buf) - 1] = '\0';

		status = regexec(&re, buf, (size_t) 0, NULL, 0);

		if(status == 0) {
			state = 1;
			break;
		}
	}

	fclose(file);

	if(checkLines()) {
		printf(MAGENTA "%s", fileDirectory);
		printf(CYAN ":");
	}

	if(state)
		return EXISTS;
	else
		return DONT_EXISTS;
}