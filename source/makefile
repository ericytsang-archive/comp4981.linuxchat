
CC   = g++ -Wall -W -Wextra -ansi -pedantic -std=c++11
# LIBS = ""



clean:
	rm -f *.o *.out



# executables
server: server.o net_helper.o select_helper.o
	$(CC) -o ./server.out server.o net_helper.o select_helper.o

client: client.o net_helper.o select_helper.o
	$(CC) -o ./client.out client.o net_helper.o select_helper.o



# main modules
server.o: server.c
	$(CC) -c server.c

client.o: client.c
	$(CC) -c client.c



# common helper modules
net_helper.o: net_helper.c net_helper.h
	$(CC) -c net_helper.c



# server helper modules
select_helper.o: select_helper.c select_helper.h
	$(CC) -c select_helper.c
