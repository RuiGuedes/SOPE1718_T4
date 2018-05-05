#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

////////////
// DEFINE //
////////////

#define SUCESS 0
#define INVALID_FUNCTION_CALL -1
#define ERROR_CREATE_FIFO -2
#define ERROR_OPEN_FIFO -3

///////////////
// FUNCTIONS //
///////////////

int createClientFifo();
