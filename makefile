CC=gcc
CFLAGS=-Wall -O2
LIBS=
OBJECTSERVER=executer.o
OUTPUTSERVER=UKSU_server
OUTPUTCLIENT=UKSU_client
OUTPUTS=UKSU_client UKSU_server

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