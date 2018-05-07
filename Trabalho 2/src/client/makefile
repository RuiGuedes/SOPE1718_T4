CC = gcc
CFLAGS = -D_GNU_SOURCE -Wall
DEPS = client.h
OBJ = client.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

client: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)   

clean: 
	rm -f client
	rm -f start
	rm -f *ans*
	rm -f $(OBJ) *.o
