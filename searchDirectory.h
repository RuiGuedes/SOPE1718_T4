#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "options.h"

#define FILE 0
#define DIRECTORY 1
#define STAT_SYSTEM_CALL_FAIL -3


//////////////////////
// Global Variables //
//////////////////////

const char * executionDirectory;
const char * pattern;

////////////////////////////
// Functions declarations //
////////////////////////////

int searchDirectory(const char * directory);
int checkFileOrDirectory();