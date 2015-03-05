
CC   = cc -Wall -W -Wextra -ansi -pedantic
# LIBS = ""



clean:
	rm -f *.o *.out



# executables
server: server.o net_helper.o
	$(CC) -o ./server.out server.o net_helper.o

client: client.o net_helper.o
	$(CC) $(LIBS) -o ./client.out client.o net_helper.o



# main modules
server.o: server.c
	$(CC) -c server.c

client.o: client.c
	$(CC) -c client.c



# common helper modules
net_helper.o: net_helper.c
	$(CC) -c net_helper.c
