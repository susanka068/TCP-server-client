all: server.cpp
	g++ -g -pthread -lpthread -o server server.cpp
clean: 
	$(RM) server
