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

  //Opens slog text file
  if(openSLOGTextFile() == FILE_OPEN_ERROR)
    return FILE_OPEN_ERROR;

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
        sem_wait(&empty);
        sem_post(&full);
      }
    }
  }

  //Terminates all threads after they execute their own requests
  terminateAllThreads(atoi(argv[2]));

  //Closes and destroys "requests" FIFO
  if(close(requests_fd) == 1)
    printf("Error while closing requests FIFO\n");

  unlink("requests");

  printServerBookings();

  fclose(slog_file);
  free(seats);

  // TODO verificar qual a melhor chamada para colocar aqui -- pthread_exit implies that the unlink requests doesnt work
  pthread_exit(NULL);

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

void terminateAllThreads(int num_threads) {

  //Change global variable
  terminate = 1;

  for(int i = 0; i < num_threads; i++)
    sem_post(&full);

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

int openSLOGTextFile() {

    int tmp_fd;

		if ((tmp_fd = open("slog.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1){
			perror("Could not open server logging text file: \n");
			return FILE_OPEN_ERROR;
		}
    else
      close(tmp_fd);

    if((slog_file = fopen("slog.txt", "w")) == NULL) {
		  perror("Could not open file: ");
		  return FILE_OPEN_ERROR;
	  }

    return SUCESS;
}

int initRoom(int num_seats) {

  //Initializes global variables
  num_room_seats = num_seats;
  num_room_seats_remaining = num_room_seats;
  seats = malloc((num_room_seats + 1)*sizeof(Seat));
  room_access_cond = malloc((num_room_seats + 1)*sizeof(pthread_cond_t));

  //Ensure that both all seats and room_access_cond components are corretly initialized
  for(int i = 0; i <= num_room_seats; i++) {
    seats[i].occupied = 0;
    seats[i].clientId = 0;
    seats[i].access_status = 0;
    pthread_cond_init(&room_access_cond[i], NULL);
  }

  return SUCESS;
}

void * ticketOffice(void * arg) {
  int tid = *(int *)arg;

  if(tid < 10)
    fprintf(slog_file, "0%d-OPEN\n", tid);
  else
    fprintf(slog_file, "%d-OPEN\n", tid);

  while(1) {
    sem_wait(&full);

    if(terminate)
      break;

    char office_request[PIPE_BUF];
    strcpy(office_request,request);

    sem_post(&empty);

    //Retrieves request separated info and validates request
    Request request_info = validateRequest(office_request, num_room_seats);

    //Request local variables
    int num_wanted_seats = request_info.num_wanted_seats;
    int reserved_seats[num_wanted_seats];
    int pref_seat_list_pointer = 0, reserved_seats_pointer = 0;

    //TODO add condition of remaining seats != num_room_seats
    while(num_wanted_seats > 0 && pref_seat_list_pointer < request_info.num_pref_seats) {

      //Loop local variables
      int seat_number = request_info.pref_seat_list[pref_seat_list_pointer];
      int access_status = seats[seat_number].access_status;

      //Checks if the wanted seat is being accessed for other thread
      pthread_mutex_lock(&access_lock);

      while(access_status)
        pthread_cond_wait(&room_access_cond[seat_number], &access_lock);

      seats[seat_number].access_status = 1;

      pthread_mutex_unlock(&access_lock);

      //Try to reserve the wanted seat
      if(isSeatFree(seats, seat_number)) {
        bookSeat(seats, seat_number, request_info.client_pid);
        reserved_seats[reserved_seats_pointer] = seat_number;
        reserved_seats_pointer++;
        num_wanted_seats--;
      }

      pref_seat_list_pointer++;
    }

    //Checks if reservation was successfully made
    if(num_wanted_seats > 0) {
        for(int i = 0; i < (request_info.num_wanted_seats - num_wanted_seats); i++)
            freeSeat(seats,reserved_seats[i]);

        request_info.validation_return_value = NAV;
    }
    else
      printRequestInfo(tid, request_info, reserved_seats);

    //Free access to all pref_seat_list seats
    for(int i = 0; i < request_info.num_pref_seats; i++) {
      seats[request_info.pref_seat_list[i]].access_status = 0;
      pthread_cond_signal(&room_access_cond[request_info.pref_seat_list[i]]);
    }

    //TODO add remaining seats number to room.h and check if room is full

    //TODO print information on slog e sbook text files


  }

  if(tid < 10)
    fprintf(slog_file, "0%d-CLOSE\n", tid);
  else
    fprintf(slog_file, "%d-CLOSE\n", tid);

  return NULL;
}

void printRequestInfo(int tid, Request request_info, int * reserved_seats) {

  char leadingZeros_pid[10];
  char leadingZeros_seat[10];
  char leadingZeros_numSeats[10];
  char width_seat[10];
  char width_pid[10];
  char width_numSeats[10];

  sprintf(width_seat, "%d", WIDTH_SEAT);
  sprintf(width_pid, "%d", WIDTH_PID);
  sprintf(width_numSeats, "%d", WIDTH_XXNN);

  strcpy(leadingZeros_pid, "%0");
  strcat(leadingZeros_pid, width_pid);
  strcat(leadingZeros_pid, "d-");

  strcpy(leadingZeros_numSeats, "%0");
  strcat(leadingZeros_numSeats, width_numSeats);
  strcat(leadingZeros_numSeats, "d: ");

  strcpy(leadingZeros_seat, "%0");
  strcat(leadingZeros_seat, width_seat);
  strcat(leadingZeros_seat, "d ");

  if(tid < 10)
    fprintf(slog_file, "0%d-", tid);
  else
    fprintf(slog_file, "0%d-", tid);

  fprintf(slog_file, leadingZeros_pid, request_info.client_pid);
  fprintf(slog_file, leadingZeros_numSeats, request_info.num_wanted_seats);

  for(int i = 0; i < MAX_CLI_SEATS; i++) {
    if(i < request_info.num_pref_seats)
      fprintf(slog_file, leadingZeros_seat, request_info.pref_seat_list[i]);
    else {
      for(int j = 0; j < WIDTH_SEAT; j++)
        fprintf(slog_file, " ");
    }
  }

  if(request_info.validation_return_value == MAX)
    fprintf(slog_file, " - MAX\n");
  else if(request_info.validation_return_value == NST)
    fprintf(slog_file, " - NST\n");
  else if(request_info.validation_return_value == IID)
    fprintf(slog_file, " - IID\n");
  else if(request_info.validation_return_value == NAV)
    fprintf(slog_file, " - NAV\n");
  else if(request_info.validation_return_value == ERR)
    fprintf(slog_file, " - ERR\n");
  else if(request_info.validation_return_value == FUL)
    fprintf(slog_file, " - FUL\n");
  else {
    fprintf(slog_file, " - ");
    for(int i = 0; i < request_info.num_wanted_seats; i++)
      fprintf(slog_file, leadingZeros_seat, reserved_seats[i]);
  }

  fprintf(slog_file, "\n");

}

int printServerBookings() {

  //Local variables
  int tmp_fd;
  FILE * sbook_file;

  if ((tmp_fd = open("sbook.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1){
    perror("Could not open server logging text file: \n");
    return FILE_OPEN_ERROR;
  }
  else
    close(tmp_fd);

  if((sbook_file = fopen("sbook.txt", "w")) == NULL) {
    perror("Could not open file: ");
    return FILE_OPEN_ERROR;
  }

  char leadingZeros_seat[10];
  char width_seat[10];

  sprintf(width_seat, "%d", WIDTH_SEAT);

  strcpy(leadingZeros_seat, "%0");
  strcat(leadingZeros_seat, width_seat);
  strcat(leadingZeros_seat, "d\n");

  for(int i = 1; i <= num_room_seats; i++) {
    if(!isSeatFree(seats,i))
      fprintf(sbook_file, leadingZeros_seat, i);
  }

  return SUCESS;

}
