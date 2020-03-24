CC=gcc
CFLAGS=-Wall -O2
LIBS=
OBJECTSERVER=executer.o
OUTPUTSERVER=server
OUTPUTCLIENT=client
OUTPUTS=server client

all:server client

server:$(OBJECTSERVER)
	$(CC) $(CFLAGS) server.c $(OBJECTSERVER) -o $(OUTPUTSERVER)

client:
	$(CC) $(CFLAGS) client.c -o $(OUTPUTCLIENT)

executer.o:
	$(CC) $(CFLAGS) -c executer.c


clean:
	rm -f *.o
	rm -f $(OUTPUTS)

rebuild:
	make clean
	make