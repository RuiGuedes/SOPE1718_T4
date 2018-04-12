#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "options.h"

#define DEFAULT   "\033[0m"

#define STAT_SYSTEM_CALL_FAIL -3
#define FILE 0
#define DIRECTORY 1

////////////////////////////
// Functions declarations //
////////////////////////////

int searchDirectory(const char * directory, int argc, char* argv[], int remainVariables);