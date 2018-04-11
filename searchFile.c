#include "searchFile.h"
#include "options.h"

int searchFile(const char * directory) {

	fileDirectory = directory;

	return searchFileWord();
}

int searchFileWord() {

	char buf[bufSize];
	int lineNumber = 1;
	int numberOfLines = 0;
	int count = 1;

	if(checkPatternExistenceOnFile() == EXISTS) {
		if(checkFileName()) {
			printf(MAGENTA "%s\n" DEFAULT, fileDirectory);
			return SUCESS;
		}
	}

	FILE *file = fopen(fileDirectory, "r");
	if(file == NULL) {
		perror("Could not open file: ");
		return FILE_OPEN_ERROR;
	}

	while (fgets(buf, sizeof(buf), file) != NULL) {	

		buf[strlen(buf)] = '\0';
		count = 1;
		
		char textLine[bufSize];
		memcpy(textLine,buf,strlen(buf));

		if(checkPatternPresenceOnTextLine(textLine) == EXISTS) {

			numberOfLines++;


			if(!checkLines()) {
				if(checkRecursivity() && (checkFileOrDirectory(executionDirectory) == 1)) {
					printf(MAGENTA "%s" DEFAULT, fileDirectory);
					printf(CYAN ":" DEFAULT);
				}

				if(checkLineNumber()) {
					printf(GREEN "%d",lineNumber);
					printf(CYAN ":");
				}
			}

			const char s[2] = " ";
			char * token;
			token = strtok(buf, s);

			while(token != NULL) {
				if(!count && !checkLines())
					printf(" ");
				else
					count = 0;
				
				analyzeAndPrintWord(token);

				token = strtok(NULL, s);
			}

		}

		lineNumber++;
	}

	if(checkLines()) {
		if(checkRecursivity() && checkFileOrDirectory(executionDirectory)) {
			printf(MAGENTA "%s", fileDirectory);
			printf(CYAN ":");
		}
		printf(DEFAULT "%d\n",numberOfLines);
	}

	fclose(file);

	return SUCESS;
}

int checkPatternExistenceOnFile() {

	char buf[bufSize];
	int status, state = -1;

	FILE *file = fopen(fileDirectory, "r");
	if(file == NULL) {
		perror("Could not open file: ");
		return FILE_OPEN_ERROR;
	}

	while (fgets(buf, sizeof(buf), file) != NULL) {	

		buf[strlen(buf) - 1] = '\0';
		char textLine[bufSize];
		memcpy(textLine,buf,bufSize);

		if(checkPatternPresenceOnTextLine(textLine) == EXISTS) {
			state = 1;
			break;
		}
	}

	fclose(file);

	if(state)
		return EXISTS;
	else
		return DONT_EXISTS;
}

int checkPatternPresenceOnTextLine(char * textLine) {

	const char s[20] = " .,!?;:()/[]\"|'\r\n\0";
	char * token;
	char * word;

	token = strtok(textLine, s);

	while(token != NULL) {
		
		if(checkICASE()) {
			if(checkCompleteWord()) {
				if(strcasecmp(token,pattern) == 0) {
					return EXISTS;
				}
			}
			else {
				if((strcasecmp(token,pattern) == 0) || ((word = strcasestr(token, pattern)) != NULL))
					return EXISTS;
			}
		}
		else {
			if(checkCompleteWord()) {
				if(strcmp(token,pattern) == 0)
					return EXISTS;
			}
			else {
				if((strcmp(token,pattern) == 0) || ((word = strstr(token,pattern)) != NULL))
					return EXISTS;
			}
		}

		token = strtok(NULL, s);
	}

	return DONT_EXISTS;
}

int analyzeAndPrintWord(char * word) {

	char * token;

	if(checkLines())
		return SUCESS;

	if(checkICASE()) {
		if(strcasecmp(word,pattern) == 0) {
			printf(BOLDRED "%s", word);
		}
		else {	
			if((token = strcasestr(word, pattern)) != NULL) {

				int size = strlen(word);
				int count = 0;

				while(size >= count) {

					char var[50];

					memcpy(var,word + count, strlen(pattern));

					if(strcasecmp(pattern,var) == 0) {
						printf(BOLDRED "%s", var);
						count += strlen(pattern);
					}
					else {
						printf(DEFAULT "%c", *(word + count));
						count++;
					}
				}
			}	
			else
				printf(DEFAULT "%s", word);
		}
	}
	else {
		if(strcmp(word,pattern) == 0) {
			printf(BOLDRED "%s", word);
		}
		else {
			if((token = strstr(word,pattern)) != NULL) {

				int size = strlen(word);
				int count = 0;

				while(size >= count) {

					char var[50];

					memcpy(var,word + count, strlen(pattern));

					if(strcmp(pattern,var) == 0) {
						printf(BOLDRED "%s", var);
						count += strlen(pattern);
					}
					else {
						printf(DEFAULT "%c", *(word + count));
						count++;
					}
				}
			}
			else {
				printf(DEFAULT "%s", word);
			}
		} 
	}
	return SUCESS;
}