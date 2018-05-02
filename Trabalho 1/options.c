#include "options.h"

int numberOptions = 0;
int availableOptions[6] = {0};	// { -l , -c , -r , -w , -i , -n }
char currentDiretory[1024];

int initOptions(int argc, char * argv[], const char * registerFileName) {

	registerExecutionFileName = registerFileName;

	printCommand(argc,argv);

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
			else 
				return -1;
		}
		else
			break;

		numberOptions++;
	}
	return numberOptions;
}

const char * setCurrentDirectory(int argc, char * argv[], int remainVariables) {
	if(remainVariables == 1) {
		if (getcwd(currentDiretory, sizeof(currentDiretory)) != NULL) {
			if(checkFileOrDirectory(currentDiretory) == 1)
				strcat(currentDiretory,"/");
			return currentDiretory;
		}
		else
			return "(INVALID DIRETORY)";
	}
	else
		return argv[argc - 1];
}

const char * setPattern(int argc, char * argv[], int remainVariables) {
	if(remainVariables == 1)
		return argv[argc - 1];
	else
		return argv[argc - 2];
}


const int checkFileName() {
	return availableOptions[0];
}


const int checkLines() {
	return availableOptions[1];
}


const int checkRecursivity() {
	return availableOptions[2];
}


const int checkCompleteWord() {
	return availableOptions[3];
}


const int checkICASE() {
	return availableOptions[4];
}

const int checkLineNumber() {
	return availableOptions[5];
}

int printCommand(int argc, char * argv[]) {

	FILE *executionRegister = fopen(registerExecutionFileName, "a+b");
	if(executionRegister == NULL) {
		perror("Could not open execution register file: \n");
		return 1; 
	}
	else {
		fprintf(executionRegister, "%.2f - %d - COMMAND ", ((double)clock()/CLOCKS_PER_SEC)*1000, getpid());
	}


	for(int i = 0; i < argc; i++) {
		fprintf(executionRegister, "%s ",argv[i]);
	}

	fprintf(executionRegister,"\n");
	fclose(executionRegister);
	return 0;
}


int printSignalRegister() {

	FILE *executionRegister = fopen(registerExecutionFileName, "a+b");
	if(executionRegister == NULL) {
		perror("Could not open execution register file: \n");
		return 1; 
	}
	else {
		fprintf(executionRegister, "%.2f - %d - %s\n", ((double)clock()/CLOCKS_PER_SEC)*1000, getpid(),"SIGNAL INT");
		fclose(executionRegister);
	}

	return 0;
}


int checkFileOrDirectory(const char * directory) {

	int file, dir;
	struct stat status;

	if(stat(directory, &status) < 0)
		return -1;

	file = S_ISREG(status.st_mode);
	dir = S_ISDIR(status.st_mode);
	
	if(file && !dir)
		return 0;
	else if(!file && dir)
		return 1;
}