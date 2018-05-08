#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

////////////
// DEFINE //
////////////

#define PIPE_BUF 4096
#define SUCESS 0
#define INVALID_FUNCTION_CALL -1
#define ERROR_CREATE_FIFO -2
#define ERROR_OPEN_FIFO -3
#define ERROR_CLOSE_FIFO -4
#define ERROR_UNLINK -5

#define WIDTH_PID 5
#define WIDTH_XXNN 5
#define WIDTH_SEAT = 4


//////////////////////
// GLOBAL VARIABLES //
//////////////////////

int time_out, num_wanted_seats;
char * pref_seat_list;

///////////////
// FUNCTIONS //
///////////////

int  openRequestsFifo();
int  initClientFifo(char * pathname);
int  terminateClientProg(char * pathname, int requests_fd, int client_fd);
int  createFormattedRequest(char * request, char * argv[]);
void initGlobalVariables(char * argv[]);
