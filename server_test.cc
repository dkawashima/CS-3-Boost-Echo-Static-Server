

#include "gtest/gtest.h"
#include "config_parser.h"
#include "echo_tcp_server.cc"

class NginxServerTest : public ::testing::Test {
protected:
	NginxConfigParser parser_;
	NginxConfig config_;
	bool ParseString(const std::string& config_string) {
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &config_);
	}
}; //Implemented Class to simplify stringstream tests.

TEST(NginxServerTest, ExampleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);
  // Makes sure parser is working correctly
  EXPECT_TRUE(success) << "example_config file did not parse successfully";
}

TEST(NginxServerTest, GetPort) {
  NginxConfigParser parser;
  NginxConfig out_config;
  parser.Parse(example_config, &out_config);
  int port = getPort(out_config);
  bool success;
  if (port == 8080){
  	success = true;
  } else {
  	success = false;
  }

  // Makes sure getPort function is working correctly
  EXPECT_TRUE(success) << "getPort function is not working correctly";
}

TEST()


