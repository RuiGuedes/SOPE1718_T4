#include "Includes.h"

////////////////////////////
// Functions declarations //
////////////////////////////

int initOptions(int argc, char * argv[]);
void setCurrentDirectory(int argc, char * argv[]);
void setPattern(int argc, char * argv[]);
void printOptionsState();

int options[6] = {0};	// { -l , -c , -r , -w , -i , -n }

int initOptions(int argc, char * argv[]) {

	int numberOptions = 0;

	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-l") == 0)
			options[0] = 1;
		else if(strcmp(argv[i], "-c") == 0)
			options[1] = 1;
		else if(strcmp(argv[i], "-r") == 0)
			options[2] = 1;
		else if(strcmp(argv[i], "-w") == 0)
			options[3] = 1;
		else if(strcmp(argv[i], "-i") == 0)
			options[4] = 1;
		else if(strcmp(argv[i], "-n") == 0)
			options[5] = 1;
		else {
			printf("Invalid function call: invalid option < %s >\n",argv[i]);
			return -1;
		}
		numberOptions++;
	}

	setCurrentDirectory(argc,argv);
	setPattern(argc,argv);
	return 0;
}

void setCurrentDirectory(int argc, char * argv[]) {
	if(options[2])
		currentDirectory = "./";
	else
		currentDirectory = argv[argc - 1];
}

void setPattern(int argc, char * argv[]) {
	if(options[2])
		pattern = argv[argc - 1];
	else
		pattern = argv[argc - 2];
}

void printOptionsState() {
	for(int j = 0; j < 6; j++)
		printf("%d",options[j]);

	printf("\n");
}