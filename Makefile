CC = g++
CFLAGS = -Wall -ansi -ggdb -std=c++0x
GTEST_DIR = gtest-1.7.0
all: webserver
# echoserver
	
echoserver:
	$(CC) $(CFLAGS) -o echoserver echo_tcp_server.cc -lboost_system -lpthread

config_parser: 
	$(CC) $(CFLAGS) -o config_parser config_parser.cc config_parser_main.cc

echotest:
	./build_tests.sh

webserver:
	$(CC) $(CFLAGS) -o webserver request.cc request_parser.cc response.cc reply.cc mime_types.cc config_parser.cc -lboost_system -lpthread

hello:
	./build_server.sh

clean: 
	rm -f webserver config_parser server_tests echoserver
