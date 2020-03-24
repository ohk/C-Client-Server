CC = gcc
CFLAGS = -O2 -Wall -pedantic
LIBS = -lm -lnsl

server: server.o executer.o
    $(CC) $(CFLAGS) $(LIBS) -o server server.o executer.o

server.o: server.c executer.h
    $(CC) $(CFLAGS) -c server.c

executer.o: executer.c executer.h
    $(CC) $(CFLAGS) -c executer.c
clean:
    rm -f test *.o

install: test
    cp test /usr/bin