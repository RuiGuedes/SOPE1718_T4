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

////////////
// DEFINE //
////////////

#define MAX_ROOM_SEATS 9999
#define MAX_CLI_SEATS 99
#define WIDTH_PID 5
#define WIDTH_XXNN 5
#define WIDTH_SEAT 4
#define IID 3
#define NST 2
#define MAX 1
#define SUCESS 0


//////////////////////
// GLOBAL VARIABLES //
//////////////////////

typedef struct Requests {
  int client_pid;
  int num_wanted_seats;
  int num_pref_seats;
  int pref_seat_list[MAX_ROOM_SEATS];
} Request;

typedef struct Seats {
    int occupied;
    int clientId;
} Seat;

int num_room_seats;
struct Seat * seats;

///////////////
// FUNCTIONS //
///////////////

int initRoom(int num_room_seats);
int validateRequest();
void initRequestStruct(char * request, Request * request_info);
int isSeatFree(Seat * seats, int seatNum);
void bookSeat(Seat * seats, int seatNum, int clientId);
void freeSeat(Seat * seats, int seatNum);
