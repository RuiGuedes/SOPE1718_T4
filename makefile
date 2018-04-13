CC = gcc
CFLAGS = -D_GNU_SOURCE
DEPS = options.h searchDirectory.h searchFile.h readFromConsole.h
OBJ = main.o options.o searchDirectory.o searchFile.o readFromConsole.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

simgrep: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm simgrep
	rm $(LOGFILENAME)
	rm -f $(OBJ) *.o