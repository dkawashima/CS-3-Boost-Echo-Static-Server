#include "gtest/gtest.h"
#include "handlers.h"
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
static std::string getBasePath(const NginxConfig &config) { // Gets base_path from config_file
  std::string pathfinder = "/";
  for (const auto& statement : config.statements_) {
    bool kl = false;
    for (const std::string& token : statement->tokens_) {
      kl = (token.find(pathfinder) != std::string::npos);
      if (kl) {
        try { return token; } catch (...) {}
      }
    }
  }
  std::string s = "No valid path!";
  return s;
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
/*
* New tests added for File/Echo Server are below.
* 
*/



TEST(NginxServerTest, GetBasePath) {
  NginxConfigParser parser;
  NginxConfig out_config;
  parser.Parse("example_config", &out_config);
  std::string base_path = getBasePath(out_config);
  bool success;
  if (base_path == "/home/user/2coolforschool"){
    success = true;
  } else {
    success = false;
  }

  // Makes sure getBasePath function is working correctly
  EXPECT_TRUE(success) << "getBasePath function is not working correctly";
}

TEST(NginxServerTest, ReadFileExtensionJPG){ // Checks if parsing file extension correctly for .jpg
  bool success;
  std::string request_path = "/static/kobe.jpg";
  std::size_t last_slash_pos = request_path.find_last_of("/");
  std::size_t last_dot_pos = request_path.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
  {
    extension = request_path.substr(last_dot_pos + 1);
  }
  std::string exp_header = http::server::mime_types::extension_to_type(extension);
  if (exp_header == "image/jpeg"){
    success = true;
  } else {
    success = false;
  }
  EXPECT_TRUE(success) << "getFileExtension procedures are not working correctly";
}
TEST(NginxServerTest, ReadFileExtensionPNG){ // Checks if parsing file extension correctly for .png
  bool success;
  std::string request_path = "/static1/jenkins.png";
  std::size_t last_slash_pos = request_path.find_last_of("/");
  std::size_t last_dot_pos = request_path.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
  {
    extension = request_path.substr(last_dot_pos + 1);
  }
  std::string exp_header = http::server::mime_types::extension_to_type(extension);
  if (exp_header == "image/png"){
    success = true;
  } else {
    success = false;
  }
  EXPECT_TRUE(success) << "getFileExtension procedures are not working correctly";
}

TEST(NginxServerTest, BadFileCall){ //Tests invalid files
  http::server::request req;
  http::server::reply rep;
  bool isEcho = false;
  bool success;
  req.uri = "/static/notkobe.jpg";
  std::string base_path = "/home/user/2coolforschool";
  http::server::request_handler reqHand(base_path);
  reqHand.http::server::request_handler::handle_request(req, rep, isEcho);

  if (rep.status == http::server::reply::not_found){
    success = true;
  } else {
    success = false;
  }
  EXPECT_TRUE(success) << "Does not display error if bad file call";
}

TEST(NginxServerTest, CorrectEchoUsage){ //Tests echo functionality
  http::server::request req;
  http::server::reply rep;
  bool isEcho = false;
  bool success;
  req.uri = "/echo/notkobe.jpg";
  std::string base_path = "/home/user/2coolforschool";
  http::server::request_handler reqHand(base_path);
  reqHand.http::server::request_handler::handle_request(req, rep, isEcho);

  if (isEcho == true){
    success = true;
  } else {
    success = false;
  }
  EXPECT_TRUE(success) << "Does not recognize echo server";
}