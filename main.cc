#include "config_parser.cc"
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
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
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), port_));
	  std::string reply = "HTTP/1.1 200 OK\r\n Content-Type: text/html\r\n\r\n <html><body>Hello, world!</body></html>\r\n";
    while (true){
		boost::system::error_code read_error;
        tcp::socket socket(io);
        acceptor.accept(socket);
		boost::asio::streambuf buffer; 
		boost::asio::read_until(socket, buffer, "\n", read_error);
		std::istream str(&buffer);
		std::string request;
		std::getline(str, request);
		if (request.find("GET") == 0){
			boost::asio::write(socket, boost::asio::buffer(reply));
		}
		std::cout << "Got here!, Port: " << port_;
		
	}
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
