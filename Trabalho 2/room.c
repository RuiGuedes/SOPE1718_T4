#include "room.h"

Request validateRequest(char * request, int num_room_seats) {

  //Local Variables
  Request request_info;
  initRequestStruct(request, &request_info);
  request_info.validation_return_value = 0;

  //Initializes global variables
  pthread_mutex_init (&access_lock, NULL);
  pthread_mutex_init (&seats_lock, NULL);

  //Validate num_wanted_seats
  if((request_info.num_wanted_seats < 1) || (request_info.num_wanted_seats > MAX_CLI_SEATS)) {
    request_info.validation_return_value = MAX;
    return request_info;
  }

  //Validate num_pref_seats
  if((request_info.num_pref_seats < request_info.num_wanted_seats) || (request_info.num_pref_seats > MAX_CLI_SEATS)) {
    request_info.validation_return_value = NST;
    return request_info;
  }

  //Validate pref_seat_list values
  for(int i = 0; i < request_info.num_pref_seats; i++) {
    if((request_info.pref_seat_list[i] < 1) || (request_info.pref_seat_list[i] > num_room_seats))
      request_info.validation_return_value = IID;
  }

  return request_info;
}

void initRequestStruct(char * request, Request * request_info) {

  //Local Variables
  char * token;
  int num_pref_seats = 0;
  int pref_seats_pointer = 0;
  const char delimiter[2] = " ";

  //Get's the first token
  token = strtok(request, delimiter);

  //Walk through other tokens
  while(token != NULL) {

    if(num_pref_seats == 0)
      (*request_info).client_pid = atoi(token);
    else if(num_pref_seats == 1)
      (*request_info).num_wanted_seats = atoi(token);
    else {
      (*request_info).pref_seat_list[pref_seats_pointer] = atoi(token);
      pref_seats_pointer++;
    }

    num_pref_seats++;
    token = strtok(NULL, delimiter);
  }

  (*request_info).num_pref_seats = num_pref_seats - 2;
}

int isSeatFree(Seat * seats, int seatNum) {

  int return_value = FREE;

  pthread_mutex_lock(&seats_lock);

  if(seats[seatNum].occupied)
    return_value = OCCUPIED;

  DELAY(DELAYED_TIME);

  pthread_mutex_unlock(&seats_lock);

  return return_value;
}

void bookSeat(Seat * seats, int seatNum, int clientId) {

  pthread_mutex_lock(&seats_lock);

  seats[seatNum].occupied = 1;
  seats[seatNum].clientId = clientId;

  DELAY(DELAYED_TIME);

  pthread_mutex_unlock(&seats_lock);
}

void freeSeat(Seat * seats, int seatNum) {

  pthread_mutex_lock(&seats_lock);

  seats[seatNum].occupied = 0;
  seats[seatNum].clientId = 0;

  DELAY(DELAYED_TIME);

  pthread_mutex_unlock(&seats_lock);
}
