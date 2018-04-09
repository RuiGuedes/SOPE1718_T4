#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "options.h"

#define STAT_SYSTEM_CALL_FAIL -3
#define FILE 0
#define DIRECTORY 1

////////////////////////////
// Functions declarations //
////////////////////////////

int searchDirectory(const char * directory);