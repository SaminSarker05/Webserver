
run: server.o client.o
	g++ server.o -o server
	g++ client.o -o client

server.o: server.cpp
	g++ -std=c++20 -c -g -Wall server.cpp

client.o: client.cpp
	g++ -std=c++20 -c -g -Wall client.cpp

.PHONY: clean
clean:
	rm *.o server client