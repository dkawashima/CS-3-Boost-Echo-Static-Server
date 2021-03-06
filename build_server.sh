#!/bin/bash

case `uname` in
  Linux) g++ main.cc -std=c++0x -g -Wall -o helloserver -lboost_system -lpthread;;
  Darwin) clang++ main.cc -std=c++11 -g -Wall -stdlib=libc++ -o webserver -I/usr/local/include;;
  *) echo "Unknown operating system";;
esac
