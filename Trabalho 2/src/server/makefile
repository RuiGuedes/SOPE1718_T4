CC = gcc
CFLAGS = -D_GNU_SOURCE -D_REENTRANT -lpthread
DEPS = server.h room.h
OBJ = server.o room.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm -f server
	rm -f requests
	rm -f $(OBJ) *.o
