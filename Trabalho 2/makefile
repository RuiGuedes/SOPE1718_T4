CC = gcc
CFLAGS = -D_GNU_SOURCE -D_REENTRANT -lpthread -lrt -Wall
DEPS = server.h room.h client.h
SERVER_OBJ = server.o room.o
CLIENT_OBJ = client.o
START_OBJ = start.o

all: server client start

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(SERVER_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

client: $(CLIENT_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

start:  $(START_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm -f client start server
	rm -f requests *ans*
	rm -f slog.txt sbook.txt clog.txt cbook.txt
	rm -f $(OBJ) *.o
