#include "gtest/gtest.h"
#include "config_parser.h"
//#include "echo_tcp_server.cc"
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

class NginxStringConfigTest : public ::testing::Test {
protected:
	NginxConfigParser parser_;
	NginxConfig config_;
	bool ParseString(const std::string& config_string) {
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &config_);
	}
}; //Implemented Class to simplify stringstream tests.

TEST(NginxConfigParserTest, ExampleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success) << "example_config file did not parse successfully";
}

TEST(NginxServerTest, GetPort) {
  NginxConfigParser parser;
  NginxConfig out_config;
  parser.Parse("example_config", &out_config);
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
TEST(NginxConfigParserTest, MediaWikiConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config1", &out_config);

  EXPECT_TRUE(success) << "MediaWikiConfig file did not parse successfully";
}

TEST(NginxConfigParserTest, FastCGIConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config2", &out_config);

  EXPECT_TRUE(success) << "FastCGIConfig file did not parse successfully";
}


TEST_F(NginxStringConfigTest, CurlyBracesConfig) {

  bool success = ParseString("foo { bar; }");
  EXPECT_TRUE(success) << "Error with curly braces";
}

TEST_F(NginxStringConfigTest, SimpleConfig){

	bool success1 = ParseString("foo bar;");

	EXPECT_TRUE(success1) << "one-line statement did not parse";
}

TEST_F(NginxStringConfigTest, TripleBracesConfig){
	bool success1 = ParseString("foo { barbaz; true { fun; } more;}}");
	EXPECT_TRUE(success1) << "Statement triple curly braces did not parse";
}

TEST(NginxConfigParserTest, ParserConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

 bool success = parser.Parse("config_parser_config", &out_config);

  EXPECT_TRUE(success) << "config_parser file did not parse successfully";
}
