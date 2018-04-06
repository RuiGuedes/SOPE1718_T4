#include <stdio.h>
#include <string.h>
#include <unistd.h>

////////////////////////////
// Functions declarations //
////////////////////////////

int initOptions(int argc, char * argv[]);
const char * setCurrentDirectory(int argc, char * argv[], int remainVariables);
const char * setPattern(int argc, char * argv[], int remainVariables);
const int checkRecursivity();
void printOptionsState();