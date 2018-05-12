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
#define FILE_OPEN_ERROR -5
#define ERROR -6

#define WIDTH_PID 5
#define WIDTH_XXNN 5
#define WIDTH_SEAT 4

#define MAX -1
#define NST -2
#define IID -3
#define ERR -4
#define NAV -5
#define FUL -6
#define OUT -7


//////////////////////
// GLOBAL VARIABLES //
//////////////////////

int time_out, num_wanted_seats;
char * pref_seat_list;

FILE * clog_file;
FILE * cbook_file;

typedef struct Answers {
    int client_pid;
    int validation_return_value;
    int * reserved_seats;
} Answer;

Answer request_answer;

///////////////
// FUNCTIONS //
///////////////

void initGlobalVariables(char * argv[]);
int  initClientFifo(char * pathname);
int  createFormattedRequest(char * request, char * argv[]);
void initializeAnswerStruct(char * answer);
int  openRequestsFifo();
int  printClientLogging();
int  printClientBookings();
void leadingZeros(char * leadingZeroString, int width_size, char * catString);
int  terminateClientProg(char * pathname, int requests_fd, int client_fd);
