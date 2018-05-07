#include "room.h"

int validateRequest(char * request) {

  //Local Variables
  struct Request request_info;
  initRequestStruct(request, &request_info);

  //Validate client request
  if((request_info.num_wanted_seats < 1) || (request_info.num_wanted_seats > MAX_CLI_SEATS))
    return MAX;
  else if((request_info.num_pref_seats < request_info.num_wanted_seats) || (request_info.num_pref_seats > MAX_CLI_SEATS))
    return NST;

  for(int i = 0; i < request_info.num_pref_seats; i++) {
    if((request_info.pref_seat_list[i] < 1) || (request_info.pref_seat_list[i] > NUM_ROOM_SEATS))
      return IID;
  }

  return SUCESS;
}

void initRequestStruct(char * request, struct Request * request_info) {
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
