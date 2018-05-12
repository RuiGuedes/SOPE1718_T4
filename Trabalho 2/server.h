#include "room.h"

////////////
// DEFINE //
////////////

#define PIPE_BUF 4096
#define SHARED 0  //sem. is shared between threads
#define INVALID_FUNCTION_CALL -1
#define ERROR_CREATE_FIFO -2
#define ERROR_OPEN_FIFO -3
#define ERROR_INIT_SEM -4
#define FILE_OPEN_ERROR -5
#define MISSING_RESOURCES -6

////////////
// COLORS //
////////////

#define DEFAULT   "\033[0m"
#define BLACK     "\033[30m"      /* Black */
#define RED       "\033[31m"      /* Red */
#define GREEN     "\033[32m"      /* Green */
#define YELLOW    "\033[33m"      /* Yellow */
#define BLUE      "\033[34m"      /* Blue */
#define MAGENTA   "\033[35m"      /* Magenta */
#define CYAN      "\033[36m"      /* Cyan */
#define WHITE     "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

//////////////////////
// GLOBAL VARIABLES //
//////////////////////

Seat * seats;

int num_room_seats;
int num_room_seats_remaining;
char request[PIPE_BUF];

FILE * slog_file;
int terminate = 0;
int client_fifo_fd;

sem_t empty, full;
pthread_cond_t * room_access_cond;

//////////////////////////////
// FUNCTIONS  - MAIN THREAD //
//////////////////////////////

int functionCallValidation(char * argv[]);
int initRoom(int num_room_seats);
int initSem();
int initRequestsFifo();
int initClientFiles();
int openSLOGTextFile();
void createTicketOffices(pthread_t * thread_ids, int num_ticket_offices);
void terminateAllThreads(int num_threads);
int terminateServerProg(int requests_fd);

//////////////////////////
// FUNCTIONS  - THREADS //
//////////////////////////

void * ticketOffice(void * arg);
int openClientFifo(Request request_info);
void sendAnswerToClient(Request request_info, int * reserved_seats);
void printServerLogging(int tid, Request request_info,int * reserved_seats);
int printServerBookings();
void leadingZeros(char * leadingZeroString, int width_size, char * catString);
