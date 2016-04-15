#include "echo_tcp_server.cc"
/* copied from yichi example lines 3-14 */
static int getPort(const NginxConfig &config) {
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

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
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
    using namespace std; // For atoi.
    //server(io_service, port_);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}