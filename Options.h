#include <stdio.h>
#include <string.h>
#include <regex.h>

////////////////////////////
// Functions declarations //
////////////////////////////

int initOptions(int argc, char * argv[]);
const char * setCurrentDirectory(int argc, char * argv[], int remainVariables);
const char * setPattern(int argc, char * argv[], int remainVariables);
void printOptionsState();