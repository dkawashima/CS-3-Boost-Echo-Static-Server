//#include "static_handler.h"
#include "request_handler.h"

//namespace http {
//namespace server {
//namespace StaticHandler {
	
class StaticHandler : public RequestHandler {

std::string directory;

bool Init(const std::map<std::string, std::string>& config_map){
	directory = config_map.at("directory");
	return 0;
}



// Handles an HTTP request. Writes the response to the HttpResponse object,
// and returns true if the request was handled successfully. If the return value
// is false, then the HttpResponse object is invalid and should not be used.
//
// Each config block has a special config named "path" that determines
// the path prefix for which the handler will be called.
//
// For example, the config "handler echo { path /echo; }" means that an echo
// handler will be called for all requests whose URI paths start with "/echo".
//
// The dispatch mechanism is implemented in the main server code.
bool HandleRequest(const HttpRequest& request, HttpResponse* response){
  // Decode url to path.
  std::string request_path;
  if (!url_decode(request.uri_, request_path))
  {
    response->status_code_ = "404";
    response->http_version_ = "1.0";
    response->reason_phrase_ = "Not Found";
    response->body_ = "HTTP/1.0 404 Not Found\r\n";
    response->headers_[0].first = "Content-Length";
    response->headers_[0].second = boost::lexical_cast<std::string>(response->body_.size());; 
    response->headers_[1].first = "Content-Type";
    response->headers_[1].second = "text/html"; 
    
    //response = responsely::stock_responsely(responsely::not_found);
    return 0;
  }

  // Request path must be absolute and not contain "..".
  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos)
  {
    response->status_code_ = "404";
    response->http_version_ = "1.0";
    response->reason_phrase_ = "Not Found";
    response->body_ = "HTTP/1.0 404 Not Found\r\n";
    response->headers_[0].first = "Content-Length";
    response->headers_[0].second = boost::lexical_cast<std::string>(response->body_.size());; 
    response->headers_[1].first = "Content-Type";
    response->headers_[1].second = "text/html"; 

    //rep = reply::stock_reply(reply::not_found);
    return 0;
  }


  // Determine the file extension.
  std::size_t last_slash_pos = request_path.find_last_of("/");
  std::size_t last_dot_pos = request_path.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
  {
    extension = request_path.substr(last_dot_pos + 1);
  }

  // Open the file to send back.
  std::string full_path = directory + request_path;
  
  /*if (full_path.find("/echo") != std::string::npos){
    isEcho = true;
    //rep = reply::stock_reply(reply::not_found); // Not returned
    return;
  } else if (full_path.find("/static") != std::string::npos && full_path.find("/static1") == std::string::npos){
    int start_position_to_erase = full_path.find("/static");
    full_path.erase(start_position_to_erase, 7);
  }*/
std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);

  if (!is)
  {
    response->status_code_ = "404";
    response->http_version_ = "1.0";
    response->reason_phrase_ = "Not Found";
    response->body_ = "HTTP/1.0 404 Not Found\r\n";
    response->headers_[0].first = "Content-Length";
    response->headers_[0].second = boost::lexical_cast<std::string>(response->body_.size());; 
    response->headers_[1].first = "Content-Type";
    response->headers_[1].second = "text/html"; 
    //rep = reply::stock_reply(reply::not_found);
    return 0;
  }


  // Fill out the reply to be sent to the client.
  std::cout << "File at path: " << full_path.c_str() << " is valid!" << "\n";
  response->status_code_ = "200";
  response->http_version_ = "1.0";
  response->reason_phrase_ = "OK";
  char buf[512];
  while (is.read(buf, sizeof(buf)).gcount() > 0){
  	response->body_.append(buf, is.gcount());
  }
  	response->headers_.resize(2);
  	response->headers_[0].first = "Content-Length";
  	response->headers_[0].second = boost::lexical_cast<std::string>(response->body_.size());
  	response->headers_[1].first = "Content-Type";
  	response->headers_[1].second = mime_types::extension_to_type(extension);

  	return 0;
}

bool url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
};

//};

/*std::string extension_to_type(const std::string& extension)
{
  for (mapping* m = mappings; m->extension; ++m)
  {
    if (m->extension == extension)
    {
      return m->mime_type;
    }
  }

  return "text/plain";
} */
};
 //}// namespace server
 //}// namespace http