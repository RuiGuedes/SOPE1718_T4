#include "room.h"

////////////
// DEFINE //
////////////

#define PIPE_BUF 4096
#define SHARED 0    //sem. is shared between threads
#define INVALID_FUNCTION_CALL -1
#define ERROR_CREATE_FIFO -2
#define ERROR_OPEN_FIFO -3
#define ERROR_INIT_SEM -4
#define FILE_OPEN_ERROR -5

//////////////////////
// GLOBAL VARIABLES //
//////////////////////

Seat * seats;

int num_room_seats;
int num_room_seats_remaining;
char request[PIPE_BUF];

FILE * slog_file;
int terminate = 0;

sem_t empty, full;
pthread_cond_t * room_access_cond;

///////////////
// FUNCTIONS //
///////////////

int initSem();
int initRequestsFifo();
int openSLOGTextFile();
int initRoom(int num_room_seats);
int functionCallValidation(char * argv[]);
void createTicketOffices(int num_ticket_offices);
void terminateAllThreads(int num_threads);
void * ticketOffice(void * arg);
void printServerLogging(int tid, Request request_info,int * reserved_seats);
int  printServerBookings();
void leadingZeros(char * leadingZeroString, int width_size, char * catString);
