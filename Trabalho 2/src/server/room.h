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

struct Request {
  int client_pid;
  int num_wanted_seats;
  int num_pref_seats;
  int pref_seat_list[MAX_ROOM_SEATS];
};

int NUM_ROOM_SEATS;


///////////////
// FUNCTIONS //
///////////////

int validateRequest();
void initRequestStruct(char * request, struct Request * request_info);
