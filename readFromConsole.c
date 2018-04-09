#include "readFromConsole.h"

int readFromConsole(const char * pattern) {

	int lineNumber = 1;

	while(1) {

		char buf[bufSize];
		fgets (buf, sizeof(buf), stdin);

		char textLine[bufSize];
		memcpy(textLine,buf,strlen(buf));
		textLine[strlen(textLine) - 1] = '\0';

		if(checkInputPatternExistence(textLine,pattern) == EXISTS) {

			if(checkFileName()) {
				printf(MAGENTA "(standard input)\n" DEFAULT);
				break;
			}
			else {
				if(checkLineNumber()) {
					printf(GREEN "%d", lineNumber);
					printf(DEFAULT ":");
				}

				const char s[7] = " .,-!?\n";
				char * token;
				token = strtok(buf, s);

				if(checkCompleteWord()) {

					while(token != NULL) {

						if(strcasecmp(token,pattern) == 0) {
							printf(BOLDRED "%s " DEFAULT, token);
						}
						else {
							printf(DEFAULT "%s ", token);
						}

						token = strtok(NULL, s);
					}
				}
				else {
					while(token != NULL) {
						analyzeWord(token, pattern, lineNumber);
						token = strtok(NULL, s);
					}
				}
				printf("\n");
			}
		}

		lineNumber++;
	}
	return SUCESS;
}

int checkInputPatternExistence(char * input, const char * pattern) {

	const char s[7] = " .,-!?\n";
	char * token;

	token = strtok(input, s);

	while(token != NULL) {
		if(checkICASE()) {
			if(checkCompleteWord()) {
				if(strcasecmp(token,pattern) == 0)
					return EXISTS;
			}
			else {
				if(strcasestr(token, pattern) != NULL)
					return EXISTS;
			}
		}
		else {
			if(checkCompleteWord()) {
				if(strcmp(token,pattern) == 0)
					return EXISTS;
			}
			else {
				if(strstr(token,pattern) != NULL)
					return EXISTS;
			}
		}

		token = strtok(NULL, s);
	}

	return DONT_EXISTS;
}