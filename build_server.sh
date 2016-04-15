#!/bin/bash

case `uname` in
  Linux) g++ echo_tcp_server.cc -std=c++0x -g -Wall -o webserver;;
  Darwin) clang++ echo_tcp_server.cc -std=c++11 -g -Wall -stdlib=libc++ -o webserver;;
  *) echo "Unknown operating system";;
esac
