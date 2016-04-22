CC = g++
CFLAGS = -Wall -ansi -pedantic -ggdb -std=c++0x
GTEST_DIR = gtest-1.7.0
all: config_parser webserver test
# echoserver
	
webserver:
	$(CC) $(CFLAGS) -o webserver echo_tcp_server.cc -lboost_system -lpthread

config_parser: 
	$(CC) $(CFLAGS) -o config_parser config_parser.cc config_parser_main.cc

test:
	./build_tests.sh

clean: 
	rm -f webserver config_parser server_tests
