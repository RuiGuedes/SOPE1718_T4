#include "client.h"

//TODO Arranjar maneira de todos os ficheiros serem criados num determinado Local

int main(int argc, char* argv[], char* envp[]) {

  //Check function validation call
  if(argc != 4) {
    printf("Usage: client <time_out> <num_wanted_seats> <pref_seat_list>\n");
    return INVALID_FUNCTION_CALL;
  }

  //Local variables declaration
  int requests_fd, client_fd;
  char pathname[8], request[PIPE_BUF];

  //Global variables initialization
  initGlobalVariables(argv);

  //Creates dedicated FIFO
  if((client_fd = initClientFifo(pathname)) == ERROR_CREATE_FIFO)
    return ERROR_CREATE_FIFO;
  else if(client_fd == ERROR_OPEN_FIFO)
    return ERROR_OPEN_FIFO;


  //Prepares request to be sent to the server
  createFormattedRequest(request, argv);

  //Opens requests fifo on write only mode
  if((requests_fd = openRequestsFifo()) == ERROR_OPEN_FIFO)
    return ERROR_OPEN_FIFO;


  //Writes request to requests fifo
  write(requests_fd, request, sizeof(request));

  //Time which client will start waiting for an anwser
  clock_t begin = clock();

  printf("BEGIN :: %ld\n",begin);

  //Main thread is responsible to listen client requests
  while( ((double)(clock() - begin) / CLOCKS_PER_SEC) < time_out) {

  }

  printf("END :: %f\n",((double)(clock() - begin) / CLOCKS_PER_SEC));

  //Close fifos
  close(requests_fd);
  close(client_fd);

  //Destroys client dedicated FIFO
  unlink(pathname);

  return(0);

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
    printf("Could not open <requests> FIFO on read only mode\n");
    return ERROR_OPEN_FIFO;
  }

  return fifo_fd;
}

void initGlobalVariables(char * argv[]) {

  //Initializes global variables
  time_out = atoi(argv[1]);
  num_wanted_seats = atoi(argv[2]);
  pref_seat_list = argv[3];

}

int  openRequestsFifo() {

  //Local variables
  int fifo_fd;

  //Opens requests fifo
  if((fifo_fd = open("../server/requests", O_WRONLY | O_NONBLOCK)) == -1) {
    printf("Could not open <requests> FIFO on write only mode\n");
    return ERROR_OPEN_FIFO;
  }

  return fifo_fd;
}

int createFormattedRequest(char * request, char * argv[]) {

  char pid[5];
  sprintf(pid,"%d",getpid());

  strcat(request,pid); strcat(request," ");
  strcat(request,argv[2]); strcat(request," ");
  strcat(request,pref_seat_list); strcat(request, "\0");

  return SUCESS;
}
