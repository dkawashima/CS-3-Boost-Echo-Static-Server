//include "echo_handler.h"
#include "request_handler.h"

//namespace http {
//namespace server {
//namespace EchoHandler {
//class Not_Found_Handler : public RequestHandler {

bool Not_Found_Handler::Init(const std::map<std::string, std::string>& config_map) {
return 0;
//Does Nothing if 404.
};



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
bool Not_Found_Handler::HandleRequest(const HttpRequest& request, HttpResponse* response){
	
	response->status_code_ = "404";
    response->http_version_ = "1.0";
    response->reason_phrase_ = "Not Found";
    response->body_ = "<html>\n"
  "<head><title>Not Found</title></head>\n"
  "<body><h1>404 Not Found</h1></body>\n"
  "</html>\n";
    std::pair<std::string, std::string> pair("Content-Length", boost::lexical_cast<std::string>(response->body_.size()));
    std::pair<std::string, std::string> pair1("Content-Type", "text/html");
    response->headers_.push_back(pair);
    response->headers_.push_back(pair1);

    //Always returns 404
    return 0;
};
//};
/*};
}
}*/