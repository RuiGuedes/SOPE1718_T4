#include "client.h"


int main(int argc, char* argv[], char* envp[]) {

  //Check function validation call
  if(argc != 4) {
    printf("Usage: client <time_out> <num_wanted_seats> <pref_seat_list>\n");
    return INVALID_FUNCTION_CALL;
  }

  //Local variables declaration
  int fifo_fd;

  //Creates dedicated FIFO
  //if(createClientFifo() == ERROR_CREATE_FIFO)
    //return ERROR_CREATE_FIFO;

  //Opens requests fifo on write only mode
  //TODO Arranjar maneira de todos os ficheiros serem criados num determinado Local
  if((fifo_fd = open("../server/requests", O_WRONLY | O_NONBLOCK)) == -1) {
    perror("Erro: ");
    printf("Could not open <requests> FIFO on write only mode\n");
    return ERROR_OPEN_FIFO;
  }


  return(0);

}

int createClientFifo() {

  char pathname[8], pid[5];

  strcpy(pathname, "ans");
  sprintf(pid, "%d", getpid());
  strcat(pathname,pid);

  if(mkfifo(pathname, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) {
    printf("Could not create <%s> FIFO\n", pathname);
    return ERROR_CREATE_FIFO;
  }

  return SUCESS;
}
