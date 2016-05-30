//
// blocking_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request_handler.h"
//#include "echo_handler.h"
//#include "static_handler.h"

using boost::asio::ip::tcp;

const int max_length = 1024;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

void session(socket_ptr sock, std::vector <std::map<std::string,std::string>> handlerVector)
{
  try
  {
    // Initialize objects at server launch
    unsigned int i = 0;
    std::string uri_path;
    HttpRequest req = {};
    EchoHandler echoHand;
    StaticHandler staticHand;
    Not_Found_Handler notHand;
    HttpResponse rep;
    request_parser rparser = request_parser();
    for (;;)
    {

      boost::array <char,8192> buffer_;
      boost::system::error_code error;
      // Get Data from user request
      std::cout << "Listening..." << "\n";
      std::size_t length = sock->read_some(boost::asio::buffer(buffer_), error);

      std::cout << "Handling request..." << "\n";
      // Parse request
      rparser.request_parser::parse(req, buffer_.data(), buffer_.data() + length);
      std::string raw_req(buffer_.begin(), buffer_.begin() + length);
      req.raw_request_ = raw_req;
      int start_position_to_erase = -1;
      // Match config file to URI (config file has 3 keys)
      for (i = 0; i < handlerVector.size(); i++){
        if (req.uri_.find(handlerVector[i]["path"]) != std::string::npos) {
        // Erase config file path from URI
        start_position_to_erase = req.uri_.find(handlerVector[i]["path"]);
        req.uri_.erase(start_position_to_erase, handlerVector[i]["path"].size());
          // If config file handler type is echo
          if (handlerVector[i]["handler"] == "echo"){
            handlerVector[i].erase("handler");
            echoHand.EchoHandler::Init(handlerVector[i]);
            echoHand.EchoHandler::HandleRequest(req, &rep);
            std::cout << "Returning echo reply..." << "\n";
          } else { // If config file handler type is static
            handlerVector[i].erase("handler");
            staticHand.StaticHandler::Init(handlerVector[i]);
            staticHand.StaticHandler::HandleRequest(req, &rep);
            std::cout << "Returning file reply..." << "\n";
          }
          break;
        } 
      }
      if (start_position_to_erase == -1){
            //notHand.Not_Found_Handler::Init("");
            notHand.Not_Found_Handler::HandleRequest(req, &rep);
            std::cout << "Returning not found reply..." << "\n";
      }



      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.
      boost::asio::write(*sock, rep.to_buffers());
      
      break;
    }

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_service& io_service, short port, std::vector <std::map<std::string,std::string>> handlerVector)
{
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    socket_ptr sock(new tcp::socket(io_service));
    a.accept(*sock);
    
    std::thread t(session, std::move(sock), handlerVector);
    t.detach();
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

// Parses child block (of handler ... {})
void getChildBlock(std::vector <std::shared_ptr<NginxConfigStatement> > statements, std::map<std::string,std::string> &map) { // Gets port from config_file
  int count = 0;
  std::string prevToken;
  for (const auto& statement : statements) {
    for (const std::string& token : statement->tokens_) {
      count = count % 2;
      if (count == 0){
        map[token] = "";
        prevToken = token;
      }
      if (count == 1){
        map[prevToken] = token;

      }
      count++;
    }
  }
  return;
}
// Parses a config file for handlers
static std::vector <std::map<std::string,std::string>> ConfigToHandlers(const NginxConfig &config) { // Gets base_path from config_file
  
  std::vector <std::map<std::string,std::string>> handMaps;
  int count = 0;
  bool kl = false;
  bool inHandler = false;
  std::map<std::string,std::string> mapToAdd;
  for (const auto& statement : config.statements_) {
    for (const std::string& token : statement->tokens_) {
      kl = (token == "handler");
      if (kl && count == 0) {
        inHandler = true;
        std::map<std::string,std::string> mapToAdd;
        mapToAdd["handler"] = "";
      }
      if (inHandler)  {
        if (count == 1){
          mapToAdd["handler"] = token; //sets handler type
          getChildBlock(statement->child_block_->statements_ ,mapToAdd);
          handMaps.push_back(mapToAdd);
          count = 0;
          inHandler = false;
        } else {
          count++;
        }
      }
  }
}
  if (handMaps.size() == 0) { // Error Message
  std::string s = "No valid handlers!";
  std::cout << s << "\n";
  } else {
    std::cout << "There are " << handMaps.size() << " valid handlers." << "\n";
  }
  return handMaps;
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

    int port_ = getPort(config);
    std::vector <std::map<std::string,std::string>> handlerVector = ConfigToHandlers(config);

    std::cout << "Server running on port: " << port_ << "\n";
    // Test ConfigToHandler function
    /*std::cout << "Vector Element 0, Key Path: " << handlerVector[0]["path"] << "\n";
    std::cout << "Vector Element 1, Key Directory: " << handlerVector[1]["directory"] << "\n"; */
    boost::asio::io_service io;
    server(io, port_, handlerVector);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}