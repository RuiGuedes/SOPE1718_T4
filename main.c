#include <stdio.h>
#include <string.h>
#include <regex.h>

#define bufSize 1024

int main(int argc, char* argv[]) {

	FILE *fp;
	char buf[bufSize];
	int lineNumber = 1;
	int    status;
    regex_t    re;

    if (regcomp(&re, "XX", REG_EXTENDED|REG_ICASE) != 0) {
        return(0);      /* Report error. */
    }


	if( (fp = fopen(argv[1], "r")) == NULL) {
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