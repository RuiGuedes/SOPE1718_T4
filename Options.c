#include "Options.h"

int availableOptions[6] = {0};	// { -l , -c , -r , -w , -i , -n }
int numberOptions = 0;

int initOptions(int argc, char * argv[]) {

	for(int i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			if(strcmp(argv[i], "-l") == 0)
				availableOptions[0] = 1;
			else if(strcmp(argv[i], "-c") == 0)
				availableOptions[1] = 1;
			else if(strcmp(argv[i], "-r") == 0)
				availableOptions[2] = 1;
			else if(strcmp(argv[i], "-w") == 0)
				availableOptions[3] = 1;
			else if(strcmp(argv[i], "-i") == 0)
				availableOptions[4] = 1;
			else if(strcmp(argv[i], "-n") == 0)
				availableOptions[5] = 1;
			else {
				printf("Invalid option. Available options: -l , -c , -r , -w , -i , -n\n");
				return -1;
			}
		}
		else
			break;
			
		numberOptions++;
	}
	return numberOptions;
}

const char * setCurrentDirectory(int argc, char * argv[], int remainVariables) {
	if(remainVariables == 1)
		return "./";
	else
		return argv[argc - 1];
}

const char * setPattern(int argc, char * argv[], int remainVariables) {
	if(remainVariables == 1)
		return argv[argc - 1];
	else
		return argv[argc - 2];
}

void printOptionsState() {
	for(int j = 0; j < 6; j++)
		printf("%d",availableOptions[j]);

	printf("\n");
}