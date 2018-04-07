#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#define bufSize 1024

////////////////////////////
// Functions declarations //
////////////////////////////

int searchFile(const char * fileDirectory, const char * pattern);