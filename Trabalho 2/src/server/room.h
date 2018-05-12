#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <dirent.h>

////////////
// DEFINE //
////////////

#define MAX_ROOM_SEATS 9999
#define MAX_CLI_SEATS 99
#define WIDTH_PID 5
#define WIDTH_XXNN 5
#define WIDTH_SEAT 4
#define WIDTH 2
#define FREE 1
#define SUCESS 0
#define OCCUPIED 0
#define MAX -1
#define NST -2
#define IID -3
#define ERR -4
#define NAV -5
#define FUL -6

#define DELAY(X) usleep(X)
#define DELAYED_TIME 50000  //Convert formula :: 1 miliseconds = 1000 microseconds

//////////////////////
// GLOBAL VARIABLES //
//////////////////////

typedef struct Requests {
  int client_pid;
  int num_wanted_seats;
  int num_pref_seats;
  int pref_seat_list[MAX_ROOM_SEATS];
  int validation_return_value;
} Request;

typedef struct Seats {
    int occupied;
    int clientId;
    int access_status;
} Seat;

pthread_mutex_t access_lock;
pthread_mutex_t seats_lock;

///////////////
// FUNCTIONS //
///////////////

Request validateRequest(char * request, int num_room_seats);
void initRequestStruct(char * request, Request * request_info);
int  isSeatFree(Seat * seats, int seatNum);
void bookSeat(Seat * seats, int seatNum, int clientId);
void freeSeat(Seat * seats, int seatNum);
