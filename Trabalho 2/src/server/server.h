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


//////////////////////
// GLOBAL VARIABLES //
//////////////////////

sem_t empty, full;
char request[PIPE_BUF];

///////////////
// FUNCTIONS //
///////////////

int initSem();
int initRequestsFifo();
int functionCallValidation(char * argv[]);
void createTicketOffices(int num_ticket_offices);
void * ticketOffice(void * arg);
