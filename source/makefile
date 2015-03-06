
CC   = g++ -Wall -W -Wextra -ansi -pedantic
# LIBS = ""



clean:
	rm -f *.o *.out



# executables
server: server.o net_helper.o
	$(CC) -o ./server.out server.o net_helper.o

client: client.o net_helper.o
	$(CC) $(LIBS) -o ./client.out client.o net_helper.o



# main modules
server.o: server.c server.h
	$(CC) -c server.c

client.o: client.c client.h
	$(CC) -c client.c



# common helper modules
net_helper.o: net_helper.c net_helper.h
	$(CC) -c net_helper.c
