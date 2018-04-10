#include "readFromConsole.h"

int readFromConsole() {

	int lineNumber = 1;

	while(1) {

		char buf[bufSize];
		fgets (buf, sizeof(buf), stdin);
		buf[strlen(buf) - 1] = '\0';

		char textLine[bufSize];
		memcpy(textLine,buf,strlen(buf));

		if(checkPatternPresenceOnTextLine(textLine) == EXISTS) {

			if(checkFileName()) {
				printf(MAGENTA "(standard input)\n" DEFAULT);
				break;
			}
			else {
				if(checkLineNumber()) {
					printf(GREEN "%d", lineNumber);
					printf(DEFAULT ":");
				}

				const char s[2] = " ";
				char * token;
				token = strtok(buf, s);

				while(token != NULL) {

					analyzeAndPrintWord(token);

					token = strtok(NULL, s);
				}
				
				printf("\n");
			}
		}

		lineNumber++;
	}

	return SUCESS;
}