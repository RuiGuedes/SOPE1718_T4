#include "server.h"

int main(int argc, char* argv[], char* envp[]) {

  //Check function validation call
  if(argc != 4) {
    printf("Usage: server <num_room_seats> <num_ticket_offices> <open_time>\n");
    return INVALID_FUNCTION_CALL;
  }

  //Checks server function call validation
  if(functionCallValidation(argv) == INVALID_FUNCTION_CALL) {
    printf("Server function call :: Invalid parameters\n");
    return INVALID_FUNCTION_CALL;
  }

  //Local variables declaration
  int requests_fd;

  //Initializes semaphores status
  if(initSem() == ERROR_INIT_SEM)
  return ERROR_INIT_SEM;

  //Initializes requests FIFO by creating and opening it
  if((requests_fd = initRequestsFifo()) == ERROR_CREATE_FIFO)
  return ERROR_CREATE_FIFO;
  else if(requests_fd == ERROR_OPEN_FIFO)
  return ERROR_OPEN_FIFO;

  //Creates num_ticket_offices threads
  createTicketOffices(atoi(argv[2]));

  //Time in which ticket offices open
  clock_t begin = clock();

  //Main thread is responsible to listen client requests
  while( ((double)(clock() - begin) / CLOCKS_PER_SEC) < atoi(argv[3])) {

    int sem_value;
    sem_getvalue(&empty, &sem_value);

    if(sem_value == 1) {
      if(read(requests_fd, request, sizeof(request)) > 0) {
        //printf("Content : %s\n", request);
        validateRequest(request);
        //sem_wait(&empty);
        //sem_post(&full);
      }
    }

  }

  // TODO verificar qual a melhor chamada para colocar aqui -- pthread_exit implies that the unlink requests doesnt work
  //pthread_exit(NULL);

  //Closes and destroys "requests" FIFO
  if(close(requests_fd) == 1)
  printf("Error while closing requests FIFO\n");


  unlink("requests");

}

int functionCallValidation(char * argv[]) {

  //Local variables declaration
  int num_seats = atoi(argv[1]);
  int num_ticket_offices = atoi(argv[2]);
  int open_time = atoi(argv[3]);

  if((num_ticket_offices <= 0) || (num_seats <= 0) || (num_seats > MAX_ROOM_SEATS) || (open_time <= 0))
    return INVALID_FUNCTION_CALL;

  return initRoom(num_seats);
}

void createTicketOffices(int num_ticket_offices) {

  // Thread id’s
  int count[num_ticket_offices];
  pthread_t thread_ids[num_ticket_offices];

  // Creates num_ticket_offices threads
  for (int k=1; k <= num_ticket_offices; k++) {
    count[k] = k;
    pthread_create(&thread_ids[k], NULL, ticketOffice, &count[k]);
  }
}

int initSem() {

  //Initializes empty semaphore
  if(sem_init(&empty, SHARED, 1) == -1) {
    printf("Errow while initializing empty semaphore\n");
    return ERROR_INIT_SEM;
  }

  //Initializes full semaphore
  if(sem_init(&full, SHARED, 0) == -1) {
    printf("Errow while initializing full semaphore\n");
    return ERROR_INIT_SEM;
  }

  return SUCESS;
}

int initRequestsFifo() {
  //Local variables
  int fifo_fd;

  //Creates fifo used to receive client requests
  if(mkfifo("requests", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) {
    printf("Could not create <requests> FIFO\n");
    return ERROR_CREATE_FIFO;
  }

  //Opens requests fifo on read-only mode
  if((fifo_fd = open("requests", O_RDONLY | O_NONBLOCK)) == -1) {
    printf("Could not open <requests> FIFO on read only mode\n");
    return ERROR_OPEN_FIFO;
  }

  return fifo_fd;
}

void * ticketOffice(void * arg) {
  int tid = *(int *)arg;

  while(1) {
    sem_wait(&full);

    char office_request[PIPE_BUF];
    strcpy(office_request,request);

    sem_post(&empty);

    printf("| %d | ------- received request :: %s\n", tid, office_request);
    sleep(5);
    printf("%d ------ Terminei\n", tid);

  }

  return NULL;
}
