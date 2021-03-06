CC = g++
CFLAGS = -Wall -ansi -ggdb -std=c++0x
GTEST_DIR = gtest-1.7.0
#use static linking




all: webserver check 
	
echoserver:
	$(CC) $(CFLAGS) -o echoserver echo_tcp_server.cc -lboost_system -lpthread

config_parser: 
	$(CC) $(CFLAGS) -o config_parser config_parser.cc config_parser_main.cc

echotest:
	./build_tests.sh

webserver:
	$(CC) $(CFLAGS) -o webserver request.cc request_parser.cc EchoHandler.cc mime_types.cc StaticHandler.cc Not_Found_Handler.cc config_parser.cc HttpResponse.cc -static-libgcc -static-libstdc++ -pthread -Wl,-Bstatic -lboost_thread -lboost_system

hello:
	./build_server.sh

test: 
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread server_test.cc config_parser.cc mime_types.cc StaticHandler.cc EchoHandler.cc reply.cc HttpResponse.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o server_tests -lboost_system -lpthread


check: test
	./server_tests

clean: 
	rm -f webserver config_parser server_tests echoserver helloserver 
