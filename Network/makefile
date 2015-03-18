
CC = g++ -Wall -W -Wextra -std=c++11
LIBS = -pthread

# clean
clean:
	rm *.out *.o



# client test modules
ClientTest: ClientTest.o Host.o select_helper.o net_helper.o
	$(CC) $(LIBS) -o ./ClientTest.out ClientTest.o Host.o select_helper.o net_helper.o

ClientTest.o: ClientTest.cpp
	$(CC) -c ClientTest.cpp




# server test modules
ServerTest: ServerTest.o Host.o select_helper.o net_helper.o
	$(CC) $(LIBS) -o ./ServerTest.out ServerTest.o Host.o select_helper.o net_helper.o

ServerTest.o: ServerTest.cpp
	$(CC) -c ServerTest.cpp




# shared helper modules
select_helper.o: select_helper.cpp select_helper.h
	$(CC) -c select_helper.cpp

net_helper.o: net_helper.cpp net_helper.h
	$(CC) -c net_helper.cpp

Host.o: Host.cpp Host.h
	$(CC) -c Host.cpp
