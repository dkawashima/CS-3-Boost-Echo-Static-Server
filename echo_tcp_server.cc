//
// blocking_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "config_parser.cc"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <thread>
#include <utility>

using boost::asio::ip::tcp;

const int max_length = 1024;

//typedef boost::shared_ptr<tcp::socket> socket_ptr;

void session(tcp::socket sock)
{
  try
  {
    for (;;)
    {
      char data[max_length];
      std::cout << "session 1" <<"\n";
      boost::system::error_code error;
      size_t length = sock.read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.
      std::cout << "session 2" << "\n";
		  
      boost::asio::write(sock, boost::asio::buffer(data, length));
      break;
    }

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_service& io_service, short port)
{
  std::cout << "End of line" << "\n";
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
  std::cout << "End of line" << "\n";
  for (;;)
  {
    /*
    boost::asio::basic_streambuf buffer;
    boost::asio::read_until(buffer, request, "\n");
    */
    std::cout << "End of line" << "\n";
    tcp::socket sock(io_service);
    std::cout << "End of line" << "\n";
    a.accept(sock);
    std::cout << "End of line" << "\n";
    //std::thread t(session, std::move(sock));
    //t.detach();
    //socket_ptr sock1 = std::move(sock);
    session(std::move(sock));
    std::cout << "End of line" << "\n";
  }
}

/* copied from yichi example lines 3-14 */
static int getPort(const NginxConfig &config) { // Gets port from config_file
  for (const auto& statement : config.statements_) {
    bool kl = true;
    for (const std::string& token : statement->tokens_) {
      if (!kl) {
        try { return stoi(token); } catch (...) {}
      }
      kl = (token != "listen");
    }
  }
  return -1;
}

int main(int argc, char* argv[])
{
  using namespace std; 
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: webserver <config_file>\n";      
      return 1;
    }
    /* copied from yichi server.main lines 17 to 21 */
    NginxConfigParser config_parser;
    NginxConfig config;
    if (!config_parser.Parse(argv[1], &config)) {
      return -1;
    }
    boost::asio::io_service io_service;
    int port_ = getPort(config);
    std::cout << port_ << "\n";
    boost::asio::io_service io;
    server(io, port_);
    std::cout << "Got here!, Port: " << port_;

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}