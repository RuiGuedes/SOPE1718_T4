#include "room.h"

int initRoom(int num_seats) {

  //Initializes room variables
  num_room_seats = num_seats;
  seats = malloc((num_room_seats + 1)*sizeof(int));

  return SUCESS;
}

int validateRequest(char * request) {

  //Local Variables
  Request request_info;
  initRequestStruct(request, &request_info);

  //Validate num_wanted_seats
  if((request_info.num_wanted_seats < 1) || (request_info.num_wanted_seats > MAX_CLI_SEATS))
    return MAX;

  //Validate num_pref_seats
  if((request_info.num_pref_seats < request_info.num_wanted_seats) || (request_info.num_pref_seats > MAX_CLI_SEATS))
    return NST;

  //Validate pref_seat_list values
  for(int i = 0; i < request_info.num_pref_seats; i++) {
    if((request_info.pref_seat_list[i] < 1) || (request_info.pref_seat_list[i] > num_room_seats))
      return IID;
  }

  return SUCESS;
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

  if((*seats))
}

void bookSeat(Seat * seats, int seatNum, int clientId) {

}

void freeSeat(Seat * seats, int seatNum) {


}
