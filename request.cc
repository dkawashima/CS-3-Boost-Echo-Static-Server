//
// blocking_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "handlers.h"

using boost::asio::ip::tcp;

const int max_length = 1024;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

namespace http {
namespace server {

void session(socket_ptr sock, std::string base_path)
{
  try
  {
    request_handler reqHand(base_path);
    request req;
    reply rep;
    bool isEcho = false;
    request_parser rparser = request_parser();
    for (;;)
    {
      boost::array <char,8192> buffer_;
      boost::system::error_code error;

      std::cout << "Listening..." << "\n";
      std::size_t length = sock->read_some(boost::asio::buffer(buffer_), error);

      std::cout << "Handling request..." << "\n";
 
      rparser.request_parser::parse(req, buffer_.data(), buffer_.data() + length);
      reqHand.request_handler::handle_request(req, rep, isEcho);



      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.
      if (isEcho == false){
        std::cout << "Returning file reply..." << "\n";
        boost::asio::write(*sock, rep.to_buffers());
      } else {
        std::cout << "Returning echo reply..." << "\n";
        boost::asio::write(*sock, boost::asio::buffer(buffer_, length));
      }
      
      break;
    }

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_service& io_service, short port, std::string base_path)
{
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    /*
    boost::asio::basic_streambuf buffer;
    boost::asio::read_until(buffer, request, "\n");
    */
    socket_ptr sock(new tcp::socket(io_service));
    a.accept(*sock);
    
    std::thread t(session, std::move(sock), base_path);
    t.detach();
    //socket_ptr sock1 = std::move(sock);
    //session(std::move(sock));
    //std::thread t(boost::bind(session, sock));
    //boost::bind(session, sock);
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
      kl = (token != "port");
    }
  }
  return -1;
}
/*
static std::map getBasePath(const NginxConfig &config) { // Gets base_path from config_file
  std::string pathfinder = "handler";
  int count = 0;
  bool kl = false;
  for (const auto& statement : config.statements_) {
    for (const std::string& token : statement->tokens_) {
      kl = (token.find(pathfinder) != std::string::npos);
      if (kl) {
        count++;
        if (count = 3) {
        try { return token; } catch (...) {}
        }
      }
    }
  }
  std::string s = "No valid path!";
  return s;
}
*/

}
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
    int port_ = http::server::getPort(config);
    std::string base_path = http::server::getBasePath(config);

    std::cout << "Server running on port: " << port_ << "\n";
    std::cout << "Base Path for files: " << base_path << "\n";
    boost::asio::io_service io;
    http::server::server(io, port_, base_path);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}