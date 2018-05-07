#include "client.h"


int main(int argc, char* argv[], char* envp[]) {

  //Check function validation call
  if(argc != 4) {
    printf("Usage: client <time_out> <num_wanted_seats> <pref_seat_list>\n");
    return INVALID_FUNCTION_CALL;
  }

  //Local variables declaration
  int fifo_fd;
  char pathname[8], request[PIPE_BUF];

  //Global variables initialization
  initGlobalVariables(argv);

  //Creates dedicated FIFO
  if(createClientFifo(pathname) == ERROR_CREATE_FIFO)
    return ERROR_CREATE_FIFO;

  //Prepares request to be sent to the server
  createFormattedRequest(request, argv);
  printf("%s\n",request);

  //Opens requests fifo on write only mode
  //TODO Arranjar maneira de todos os ficheiros serem criados num determinado Local
  if((fifo_fd = open("../server/requests", O_WRONLY | O_NONBLOCK)) == -1) {
    perror("Erro: ");
    printf("Could not open <requests> FIFO on write only mode\n");
    return ERROR_OPEN_FIFO;
  }

  write(fifo_fd, request, sizeof(request));

  close(fifo_fd);

  //Destroys client dedicated FIFO
  unlink(pathname);

  return(0);

}

int createClientFifo(char * pathname) {

  char pid[5];
  sprintf(pid, "%d", getpid());

  strcpy(pathname, "ans");
  strcat(pathname,pid);

  if(mkfifo(pathname, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) {
    printf("Could not create <%s> FIFO\n", pathname);
    return ERROR_CREATE_FIFO;
  }

  return SUCESS;
}

void initGlobalVariables(char * argv[]) {

  time_out = atoi(argv[1]);
  num_wanted_seats = atoi(argv[2]);
  pref_seat_list = argv[3];

}

int createFormattedRequest(char * request, char * argv[]) {

  char pid[5];
  sprintf(pid,"%d",getpid());

  strcat(request,pid); strcat(request," ");
  strcat(request,argv[2]); strcat(request," ");
  strcat(request,pref_seat_list); strcat(request, "\0");

  return SUCESS;
}
