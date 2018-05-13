#include "client.h"

int main(int argc, char* argv[], char* envp[]) {

  //Check function validation call
  if(argc != 4) {
    printf("Usage: client <time_out> <num_wanted_seats> <pref_seat_list>\n");
    return INVALID_FUNCTION_CALL;
  }

  //Local variables declaration
  int requests_fd, client_fd;
  char pathname[50], request[PIPE_BUF], answer[PIPE_BUF];

  //Global variables initialization
  initGlobalVariables(argv);

  //Opens requests fifo on write only mode
  if((requests_fd = openRequestsFifo()) == ERROR_OPEN_FIFO)
    return ERROR_OPEN_FIFO;

  //Creates dedicated FIFO
  if((client_fd = initClientFifo(pathname)) == ERROR_CREATE_FIFO)
    return ERROR_CREATE_FIFO;
  else if(client_fd == ERROR_OPEN_FIFO)
    return ERROR_OPEN_FIFO;

  //Prepares request to be sent to the server
  createFormattedRequest(request, argv);

  //Writes request to requests fifo
  if(write(requests_fd, request, sizeof(request)) < 0)
    return ERROR;

  //Time which client will start waiting for an anwser
  clock_t begin = clock();

  //Waits for an answer
  while( (((double)(clock() - begin) / CLOCKS_PER_SEC) * 1000) < time_out) {

    if(read(client_fd, answer, sizeof(answer)) > 0) {
        initializeAnswerStruct(answer);
        printClientLogging();
        printClientBookings();
        return terminateClientProg(pathname,requests_fd,client_fd);
    }
  }

  //Time out response
  request_answer.client_pid = getpid();
  request_answer.validation_return_value = -7;
  printClientLogging();

  //Terminate client program
  return terminateClientProg(pathname,requests_fd,client_fd);
}

void initGlobalVariables(char * argv[]) {

  //Initializes global variables
  time_out = atoi(argv[1]);
  num_wanted_seats = atoi(argv[2]);
  pref_seat_list = argv[3];

}

int initClientFifo(char * pathname) {

  //Local variables
  int fifo_fd;
  char pid[5];
  sprintf(pid, "%d", getpid());

  strcpy(pathname, "ans");
  strcat(pathname,pid);

  //Creates client fifo
  if(mkfifo(pathname, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) {
    printf("Could not create <%s> FIFO\n", pathname);
    return ERROR_CREATE_FIFO;
  }

  //Opens client fifo on write-only mode
  if((fifo_fd = open(pathname, O_RDONLY | O_NONBLOCK)) == -1) {
    printf("Could not open <%s> FIFO on read only mode\n", pathname);
    return ERROR_OPEN_FIFO;
  }

  return fifo_fd;
}

int createFormattedRequest(char * request, char * argv[]) {

  char pid[WIDTH_PID];
  sprintf(pid,"%d",getpid());

  strcat(request,pid); strcat(request," ");
  strcat(request,argv[2]); strcat(request," ");
  strcat(request,pref_seat_list); strcat(request, "\0");

  return SUCESS;
}

void initializeAnswerStruct(char * answer) {

  //Local Variables
  char * token;
  int type = 0, reserved_seats_pointer = 0;
  const char delimiter[4] = " \n";

  //Initializes seats array of request_answer struct
  request_answer.reserved_seats = malloc(num_wanted_seats*sizeof(int));

  //Get's the first token
  token = strtok(answer, delimiter);

  //Walk through other tokens
  while(token != NULL) {

    if(type == 0)
      request_answer.client_pid = atoi(token);
    else if(type == 1)
      request_answer.validation_return_value = atoi(token);
    else {
      request_answer.reserved_seats[reserved_seats_pointer] = atoi(token);
      reserved_seats_pointer++;
    }

    type++;
    token = strtok(NULL, delimiter);
  }

}

int openRequestsFifo() {

  //Local variables
  int fifo_fd;

  //Opens requests fifo
  if((fifo_fd = open("requests", O_WRONLY)) == -1) {
    printf("Could not open <requests> FIFO on write only mode\n");
    return ERROR_OPEN_FIFO;
  }

  return fifo_fd;
}

int printClientLogging() {

  //Local variables
  char leadingZeros_pid[10];
  char leadingZeros_xxnn[10];
  char leadingZeros_seat[10];

  if((clog_file = fopen("clog.txt", "a")) == NULL) {
    perror("Could not open file: ");
    return FILE_OPEN_ERROR;
  }

  //Initializes local variables
  leadingZeros(leadingZeros_pid,WIDTH_PID,"d ");
  leadingZeros(leadingZeros_xxnn,(WIDTH_XXNN - 1)/2,"d");
  leadingZeros(leadingZeros_seat,WIDTH_SEAT,"d ");


  //Prints result of request
  if(request_answer.validation_return_value != 0) {

    //Print client PID
    if(request_answer.client_pid != 0)
      fprintf(clog_file, leadingZeros_pid, request_answer.client_pid);

    if(request_answer.validation_return_value == MAX)
      fprintf(clog_file, "MAX\n");
    else if(request_answer.validation_return_value == NST)
      fprintf(clog_file, "NST\n");
    else if(request_answer.validation_return_value == IID)
      fprintf(clog_file, "IID\n");
    else if(request_answer.validation_return_value == NAV)
      fprintf(clog_file, "NAV\n");
    else if(request_answer.validation_return_value == ERR)
      fprintf(clog_file, "ERR\n");
    else if(request_answer.validation_return_value == FUL)
      fprintf(clog_file, "FUL\n");
    else if(request_answer.validation_return_value == OUT)
      fprintf(clog_file, "OUT\n");
  }
  else {

    for(int i = 1; i <= num_wanted_seats; i++) {
      fprintf(clog_file, leadingZeros_pid, request_answer.client_pid);
      fprintf(clog_file, leadingZeros_xxnn, i);
      fprintf(clog_file, ".");
      fprintf(clog_file, leadingZeros_xxnn, num_wanted_seats);
      fprintf(clog_file, " ");
      fprintf(clog_file, leadingZeros_seat, request_answer.reserved_seats[i-1]);
      fprintf(clog_file, "\n");
    }
  }

  //Closes clog_file
  fclose(clog_file);

  return SUCESS;

}

int printClientBookings() {

  //Local variables
  char leadingZeros_seat[10];

  if((cbook_file = fopen("cbook.txt", "a")) == NULL) {
    perror("Could not open file: ");
    return FILE_OPEN_ERROR;
  }

  //Initializes local variables
  leadingZeros(leadingZeros_seat,WIDTH_SEAT,"d");

  //Prints result of request
  if(request_answer.validation_return_value == 0) {

    for(int i = 1; i <= num_wanted_seats; i++) {
      fprintf(cbook_file, leadingZeros_seat, request_answer.reserved_seats[i-1]);
      fprintf(cbook_file, "\n");
    }
  }

  //Closes cbook_file
  fclose(cbook_file);

  return SUCESS;
}

void leadingZeros(char * leadingZeroString, int width_size, char * catString) {

  //Local variables
  char width[10];

  //Format string
  sprintf(width, "%d", width_size);
  strcpy(leadingZeroString, "%0");
  strcat(leadingZeroString, width);
  strcat(leadingZeroString, catString);
}

int terminateClientProg(char * pathname, int requests_fd, int client_fd) {

  //Closes requests fifo
  if(close(requests_fd) == -1) {
    printf("Error while closing requests fifo\n");
    return ERROR_CLOSE_FIFO;
  }

  //Closes client dedicated fifo
  if(close(client_fd) == -1) {
    printf("Error while closing client fifo\n");
    return ERROR_CLOSE_FIFO;
  }

  //Destroys client dedicated fifo
  if(unlink(pathname) == -1) {
    printf("Error while unlinking %s fifo\n", pathname);
    return ERROR_UNLINK;
  }

  free(request_answer.reserved_seats);

  return SUCESS;
}
