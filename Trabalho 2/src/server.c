#include "server.h"

int main(int argc, char* argv[], char* envp[]) {

  //Check function validation call
  if(argc != 4) {
    printf("Usage: server <num_room_seats> <num_ticket_offices> <open_time>\n");
    return INVALID_FUNCTION_CALL;
  }

  //Local variables declaration
  int fifo_fd;

  //Creates requests FIFO
  if(mkfifo("requests", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) {
    printf("Could not create <requests> FIFO\n");
    return ERROR_CREATE_FIFO;
  }

  //Opens requests fifo read-only
  if((fifo_fd = open("requests", O_RDONLY | O_NONBLOCK)) == -1) {
    printf("Could not open <requests> FIFO on read only mode\n");
    return ERROR_OPEN_FIFO;
  }

  //Creates num_ticket_offices threads
  createTicketOffices(atoi(argv[2]));

  //Time in which ticket offices open
  clock_t begin = clock();

  //Main thread is responsible to listen client requests
  while( ((double)(clock() - begin) / CLOCKS_PER_SEC) < atoi( argv[3])) {

  }

  // TODO verificar qual a melhor chamada para colocar aqui
  pthread_exit(NULL);

  //Closes and destroys "requests" FIFO
  close(fifo_fd);
  unlink("requests");

}


void createTicketOffices(int num_ticket_offices) {
  // Thread id’s
  pthread_t thread_ids[num_ticket_offices];

  // Creates num_ticket_offices threads
  for (int k=0; k < num_ticket_offices; k++)
    pthread_create(&thread_ids[k], NULL, ticketOffice, NULL);
}

void * ticketOffice(void * arg)
{
  printf("Ticket Office created\n");
  return NULL;
}
