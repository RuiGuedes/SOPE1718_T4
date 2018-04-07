#include <stdio.h>
#include <string.h>
#include <unistd.h>

////////////////////////////
// Functions declarations //
////////////////////////////

int initOptions(int argc, char * argv[]);
const char * setCurrentDirectory(int argc, char * argv[], int remainVariables);
const char * setPattern(int argc, char * argv[], int remainVariables);

const int checkFileName();
const int checkLines();
const int checkRecursivity();
const int checkCompleteWord();
const int checkICASE();
const int checkLineNumber();

void printOptionsState();