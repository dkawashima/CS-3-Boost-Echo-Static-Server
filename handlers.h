#include "config_parser.h"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <thread>
#include <utility>
#include <string>
#include <map>
#include "request_parser.h"
#include <fstream>
//#include "request_handler.h"

namespace http {
namespace server {

struct header
{
  std::string name;
  std::string value;
};
struct reply
{

  /// The status of the reply.
  enum status_type
  {
    ok = 200,
    not_found = 404,

  } status;

  /// The headers to be included in the reply.
  std::vector<header> headers;

  /// The content to be sent in the reply.
  std::string content;

  /// Convert the reply into a vector of buffers. The buffers do not own the
  /// underlying memory blocks, therefore the reply object must remain valid and
  /// not be changed until the write operation has completed.
  std::vector<boost::asio::const_buffer> to_buffers();

  /// Get a stock reply.
  static reply stock_reply(status_type status);
};
struct request {
  std::string method;
  std::string uri;
  int http_version_major;
  int http_version_minor;
  std::vector<header> headers;
};

/// The common handler for all incoming requests.
class request_handler
  : private boost::noncopyable
{
public:
  /// Construct with a directory containing files to be served.
  explicit request_handler(const std::string& doc_root);

  /// Handle a request and produce a reply.
  void handle_request(const request& req, reply& rep, bool& isEcho);

private:
  /// The directory containing the files to be served.
  std::string doc_root_;

  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool url_decode(const std::string& in, std::string& out);
};
namespace mime_types {

/// Convert a file extension into a MIME type.
std::string extension_to_type(const std::string& extension);

} // namespace mime_types

/*void session(socket_ptr sock, std::string base_path);
void server(boost::asio::io_service& io_service, short port, std::string base_path);
static std::string getBasePath(const NginxConfig &config);
static int getPort(const NginxConfig &config);
*/
//std::vector<boost::asio::const_buffer> HttpResponse::to_buffers();
} // namespace server
} // namespace http