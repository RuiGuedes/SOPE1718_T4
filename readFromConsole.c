#include "readFromConsole.h"

void sigint_handler(int signum) {
 
 	char answer;
 	printf(" - Are you sure you want to terminate the program? (Y/N) ");
 	scanf("%c", &answer);

 	if(answer == 'Y')
 		exit(0);

}


int readFromConsole() {

	int lineNumber = 1;
	
	struct sigaction action;
	action.sa_handler = sigint_handler;

	if (sigaction(SIGINT, &action, NULL) < 0) {
		perror ("Sigaction: ");
		return 1;
	}

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
					printf(" ");
					token = strtok(NULL, s);
				}
				
				printf("\n");
			}
		}

		lineNumber++;
	}

	return SUCESS;
}