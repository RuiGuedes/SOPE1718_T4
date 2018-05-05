CC = gcc
CFLAGS = -D_GNU_SOURCE -D_REENTRANT -lpthread
DEPS = server.h 
OBJ = server.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm server
	rm requests
	rm -f $(OBJ) *.o
