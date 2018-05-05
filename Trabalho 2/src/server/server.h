#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

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

void createTicketOffices(int num_ticket_offices);
void * ticketOffice(void * arg);
