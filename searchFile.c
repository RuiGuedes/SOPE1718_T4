#include "searchFile.h"

int searchFile(const char * fileDirectory, const char * pattern) {

	char buf[bufSize];
	int lineNumber = 1;
	int    status;
    regex_t    re;

    if (regcomp(&re, pattern, REG_EXTENDED|REG_ICASE) != 0) {
        return(0);      // Report error.
    }


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
    		printf("%d:%s\n",lineNumber, buf);
    	}

    	lineNumber++;
	}

	regfree(&re);
	fclose(file);

	return 0;

}