#include "server.h"

int main(int argc, char* argv[], char* envp[]) {

  //Check function validation call
  if(argc != 4) {
    printf("Usage: server <num_room_seats> <num_ticket_offices> <open_time>\n");
    return INVALID_FUNCTION_CALL;
  }

  DIR* dir = opendir("../../resources");
  if (dir)
    closedir(dir);
  else {
    printf("Error: Folder <resources> must be created at same level as <src> folder\n");
    return MISSING_RESOURCES;
  }

  //Checks server function call validation
  if(functionCallValidation(argv) == INVALID_FUNCTION_CALL) {
    printf("Server function call :: Invalid parameters\n");
    return INVALID_FUNCTION_CALL;
  }

  //Local variables declaration
  int requests_fd;
  pthread_t thread_ids[atoi(argv[2])];

  //Initializes semaphores status
  if(initSem() == ERROR_INIT_SEM)
    return ERROR_INIT_SEM;

  //Initializes requests FIFO by creating and opening it
  if((requests_fd = initRequestsFifo()) == ERROR_CREATE_FIFO)
    return ERROR_CREATE_FIFO;
  else if(requests_fd == ERROR_OPEN_FIFO)
    return ERROR_OPEN_FIFO;

  if(initClientFiles() < 0)
    return FILE_OPEN_ERROR;

  //Opens slog text file
  if(openSLOGTextFile() == FILE_OPEN_ERROR)
    return FILE_OPEN_ERROR;

  //Creates num_ticket_offices threads
  createTicketOffices(thread_ids, atoi(argv[2]));

  //Time in which ticket offices open
  clock_t begin = clock();

  printf("\nServer opened !\n\n");

  //Main thread is responsible to listen client requests
  while( ((double)(clock() - begin) / CLOCKS_PER_SEC) < atoi(argv[3])) {

    int sem_value;
    sem_getvalue(&empty, &sem_value);

    if(sem_value == 1) {
      if(read(requests_fd, request, sizeof(request)) > 0) {
        printf("Received request :: %s\n", request);
        sem_wait(&empty);
        sem_post(&full);
      }
    }
  }

  //Closes and destroys "requests" FIFO
  if(close(requests_fd) == 1)
    printf("Error while closing requests FIFO\n");

  //Destroys requests fifo
  unlink("../../resources/requests");

  printf("\nServer closed ! Handling remaining requests\n\n");

  //Terminates all threads after they execute their own requests
  terminateAllThreads(atoi(argv[2]));

  //Wait's for all threads
  for(int i = 1; i <= atoi(argv[2]); i++)
      pthread_join(thread_ids[i], NULL);


  //Prints information on sbook_file
  printServerBookings();

  //Prints last message on slog_file
  fprintf(slog_file, "SERVER CLOSED\n");

  return terminateServerProg(requests_fd);
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
  if(mkfifo("../../resources/requests", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) {
    printf("Could not create <requests> FIFO\n");
    return ERROR_CREATE_FIFO;
  }

  //Opens requests fifo on read-only mode
  if((fifo_fd = open("../../resources/requests", O_RDONLY | O_NONBLOCK)) == -1) {
    printf("Could not open <requests> FIFO on read only mode\n");
    return ERROR_OPEN_FIFO;
  }

  return fifo_fd;
}

int initClientFiles() {

  //Local variables
  int tmp_fd;

  if ((tmp_fd = open("../../resources/clog.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1){
    perror("Could not open server logging text file: \n");
    return FILE_OPEN_ERROR;
  }
  else
    close(tmp_fd);

  if ((tmp_fd = open("../../resources/cbook.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1){
    perror("Could not open server logging text file: \n");
    return FILE_OPEN_ERROR;
  }
  else
    close(tmp_fd);

  return SUCESS;
}

int openSLOGTextFile() {

    int tmp_fd;

		if ((tmp_fd = open("../../resources/slog.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1){
			perror("Could not open server logging text file: \n");
			return FILE_OPEN_ERROR;
		}
    else
      close(tmp_fd);

    if((slog_file = fopen("../../resources/slog.txt", "w")) == NULL) {
		  perror("Could not open server logging file: ");
		  return FILE_OPEN_ERROR;
	  }

    return SUCESS;
}

void createTicketOffices(pthread_t * thread_ids, int num_ticket_offices) {

  // Thread id’s
  int count[num_ticket_offices];

  // Creates num_ticket_offices threads
  for (int k=1; k <= num_ticket_offices; k++) {
    count[k-1] = k;
    pthread_create(&thread_ids[count[k-1]], NULL, ticketOffice, &count[k-1]);
  }

  DELAY(DELAYED_TIME);
}

void terminateAllThreads(int num_threads) {

  //Change global variable
  terminate = 1;

  for(int i = 0; i < num_threads; i++)
    sem_post(&full);

}

int terminateServerProg(int requests_fd) {

  //Destroys semaphores
  sem_close(&empty);
  sem_close(&full);

  //Destroys mutexes
  pthread_mutex_destroy (&access_lock);
  pthread_mutex_destroy (&seats_lock);

  //Closes slog_file
  fclose(slog_file);

  //Liberta o espaço de seats e de room access
  free(seats);
  free(room_access_cond);

  return SUCESS;
}

void * ticketOffice(void * arg) {

  int tid = *(int *)arg;
  fprintf(slog_file, "%02d-OPEN\n", tid);

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

    //Check if room is full or not
    if(num_room_seats_remaining == 0)
      request_info.validation_return_value = FUL;

    while(num_wanted_seats > 0 && pref_seat_list_pointer < request_info.num_pref_seats && request_info.validation_return_value == 0) {

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

        if(request_info.validation_return_value == 0)
          request_info.validation_return_value = NAV;
    }

    //Opens client FIFO to send answer
    client_fifo_fd = openClientFifo(request_info);

    //Stores request information on slog text file
    sendAnswerToClient(request_info, reserved_seats);
    printServerLogging(tid, request_info, reserved_seats);

    //Free access to all pref_seat_list seats
    for(int i = 0; i < request_info.num_pref_seats; i++) {
      seats[request_info.pref_seat_list[i]].access_status = 0;
      pthread_cond_signal(&room_access_cond[request_info.pref_seat_list[i]]);
    }
  }

  fprintf(slog_file, "%02d-CLOSE\n", tid);

  return NULL;
}

int openClientFifo(Request request_info) {

  //Local variables
  int fifo_fd;
  char pathname[100], pid[10];

  //Initializes client fifo name
  sprintf(pid, "%d", request_info.client_pid);
  strcpy(pathname, "../../resources/ans");
  strcat(pathname,pid);

  //Opens requests fifo on read-only mode
  if((fifo_fd = open(pathname, O_WRONLY)) == -1) {
    perror("Could not open client fifo on write only mode: ");
    //printf("Could not open client fifo %s on write only mode\n", pathname);
    return ERROR_OPEN_FIFO;
  }

  return fifo_fd;
}

void sendAnswerToClient(Request request_info, int * reserved_seats) {

  //Local variables
  char answer[PIPE_BUF], pid[WIDTH_PID], return_value[WIDTH_PID];

  //Initializes client fifo name
  sprintf(pid, "%d", request_info.client_pid);
  sprintf(return_value, "%d", request_info.validation_return_value);

  strcpy(answer, "");
  strcat(answer,pid); strcat(answer," ");
  strcat(answer,return_value); strcat(answer," ");

  if(request_info.validation_return_value == 0) {
      for(int i = 0; i < request_info.num_wanted_seats; i++) {
        char seat_number[WIDTH_SEAT];
        sprintf(seat_number, "%d", reserved_seats[i]);
        strcat(answer,seat_number);
        strcat(answer," ");
      }
  }

  //Appends new line character
  strcat(answer,"\n");

  //Writes answer to client fifo
  write(client_fifo_fd, answer, sizeof(answer));

  close(client_fifo_fd);
}

void printServerLogging(int tid, Request request_info, int * reserved_seats) {

  //Local variables
  char leadingZeros_tid[10];
  char leadingZeros_pid[10];
  char leadingZeros_numSeats[10];
  char leadingZeros_seat[10];


  //Initializes local variables
  leadingZeros(leadingZeros_tid, WIDTH,"d-");
  leadingZeros(leadingZeros_pid,WIDTH_PID,"d-");
  leadingZeros(leadingZeros_numSeats,WIDTH,"d: ");
  leadingZeros(leadingZeros_seat,WIDTH_SEAT,"d ");

  //Prints initial information
  fprintf(slog_file, leadingZeros_tid, tid);
  fprintf(slog_file, leadingZeros_pid, request_info.client_pid);
  fprintf(slog_file, leadingZeros_numSeats, request_info.num_wanted_seats);

  //Prints pref_seat_list
  for(int i = 0; i < MAX_CLI_SEATS; i++) {
    if(i < request_info.num_pref_seats)
      fprintf(slog_file, leadingZeros_seat, request_info.pref_seat_list[i]);
    else {
      for(int j = 0; j < WIDTH_SEAT; j++) {
        if((j+1) == WIDTH_SEAT)
          fprintf(slog_file, "  ");
        else
          fprintf(slog_file, " ");
      }
    }
  }

  //Prints result of request
  fprintf(slog_file, "- ");

  if(request_info.validation_return_value == MAX)
    fprintf(slog_file, "MAX");
  else if(request_info.validation_return_value == NST)
    fprintf(slog_file, "NST");
  else if(request_info.validation_return_value == IID)
    fprintf(slog_file, "IID");
  else if(request_info.validation_return_value == NAV)
    fprintf(slog_file, "NAV");
  else if(request_info.validation_return_value == ERR)
    fprintf(slog_file, "ERR");
  else if(request_info.validation_return_value == FUL)
    fprintf(slog_file, "FUL");
  else {
    for(int i = 0; i < request_info.num_wanted_seats; i++)
      fprintf(slog_file, leadingZeros_seat, reserved_seats[i]);
  }

  //Prints newline character
  fprintf(slog_file, "\n");
}

int printServerBookings() {

  //Local variables
  int tmp_fd;
  char leadingZeros_seat[10];
  FILE * sbook_file;

  if ((tmp_fd = open("../../resources/sbook.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1){
    perror("Could not open server logging text file: \n");
    return FILE_OPEN_ERROR;
  }
  else
    close(tmp_fd);

  if((sbook_file = fopen("../../resources/sbook.txt", "w")) == NULL) {
    perror("Could not open file: ");
    return FILE_OPEN_ERROR;
  }

  //Initializes formatted leadingZeros expression
  leadingZeros(leadingZeros_seat,WIDTH_SEAT,"d\n");

  for(int i = 1; i <= num_room_seats; i++) {
    if(!isSeatFree(seats,i))
      fprintf(sbook_file, leadingZeros_seat, i);
  }

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
