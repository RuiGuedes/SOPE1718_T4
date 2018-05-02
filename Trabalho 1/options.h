#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <termios.h>
#include <time.h>

//////////////////////
// Global Variables //
//////////////////////

const char* registerExecutionFileName;
const char * executionDirectory;
const char * pattern;

////////////////////////////
// Functions declarations //
////////////////////////////

int initOptions(int argc, char * argv[], const char * registerFileName);
const char * setCurrentDirectory(int argc, char * argv[], int remainVariables);
const char * setPattern(int argc, char * argv[], int remainVariables);

const int checkFileName();
const int checkLines();
const int checkRecursivity();
const int checkCompleteWord();
const int checkICASE();
const int checkLineNumber();

int printCommand(int argc, char * argv[]);
int printSignalRegister();
int checkFileOrDirectory(const char * directory);