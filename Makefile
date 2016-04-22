CC = g++
CFLAGS = -Wall -ansi -pedantic -ggdb -std=c++0x

all: webserver 
# echoserver
	
webserver:
	$(CC) $(CFLAGS) -o webserver echo_tcp_server.cc -lboost_system -lpthread

config_parser: 
	$(CC) $(CFLAGS) -o config_parser config_parser.cc config_parser_main.cc

# echoserver:
#$(CC) $(CFLAGS) -o echoserver echo_tcp_server.cc -lboost_system

clean: 
	rm -f webserver
