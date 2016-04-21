CC = g++
CFLAGS = -Wall -ansi -pedantic -ggdb -std=c++0x

all:
	$(CC) $(CFLAGS) -o webserver main.cc -lboost_system

config_parser: 
	$(CC) $(CFLAGS) -o config_parser config_parser.cc config_parser_main.cc -lboost_system

clean: 
	rm webserver
