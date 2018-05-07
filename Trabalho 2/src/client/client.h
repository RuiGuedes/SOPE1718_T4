#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

////////////
// DEFINE //
////////////

#define PIPE_BUF 4096
#define SUCESS 0
#define INVALID_FUNCTION_CALL -1
#define ERROR_CREATE_FIFO -2
#define ERROR_OPEN_FIFO -3

//////////////////////
// GLOBAL VARIABLES //
//////////////////////

int time_out, num_wanted_seats;
char * pref_seat_list;

///////////////
// FUNCTIONS //
///////////////

int  createClientFifo(char * pathname);
int  createFormattedRequest(char * request, char * argv[]);
void initGlobalVariables(char * argv[]);
