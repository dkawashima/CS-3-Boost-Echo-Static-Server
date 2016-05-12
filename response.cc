//
// blocking_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "request_handlers.h"

namespace http {
namespace server {


//TODO
virtual bool request_handler::init(const std::map<std::string, std::string>& config_map){
    return 0;
}
  


virtual bool request_handler::handle_request(const HttpRequest& req, HttpResponse* rep)
{
  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.uri, request_path))
  {
    rep.status_code_ = "404";
    rep.http_version_ = "1.0";
    rep.reason_phrase_ = "Not Found";
    rep.body_ = "HTTP/1.0 404 Not Found\r\n";
    rep.headers[0].first = "Content-Length";
    rep.headers[0].second = boost::lexical_cast<std::string>(rep.body_.size());; 
    rep.headers[1].first = "Content-Type";
    rep.headers[1].second = "text/html"; 
    
    //rep = reply::stock_reply(reply::not_found);
    return;
  }

  // Request path must be absolute and not contain "..".
  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos)
  {
    rep.status_code_ = "404";
    rep.http_version_ = "1.0";
    rep.reason_phrase_ = "Not Found";
    rep.body_ = "HTTP/1.0 404 Not Found\r\n";
    rep.headers[0].first = "Content-Length";
    rep.headers[0].second = boost::lexical_cast<std::string>(rep.body_.size());; 
    rep.headers[1].first = "Content-Type";
    rep.headers[1].second = "text/html"; 

    //rep = reply::stock_reply(reply::not_found);
    return;
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
  std::string full_path = doc_root_ + request_path;
  
  if (full_path.find("/echo") != std::string::npos){
    isEcho = true;
    //rep = reply::stock_reply(reply::not_found); // Not returned
    return;
  } else if (full_path.find("/static") != std::string::npos && full_path.find("/static1") == std::string::npos){
    int start_position_to_erase = full_path.find("/static");
    full_path.erase(start_position_to_erase, 7);
  }
std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);

  if (!is)
  {
    rep.status_code_ = "404";
    rep.http_version_ = "1.0";
    rep.reason_phrase_ = "Not Found";
    rep.body_ = "HTTP/1.0 404 Not Found\r\n";
    rep.headers[0].first = "Content-Length";
    rep.headers[0].second = boost::lexical_cast<std::string>(rep.body_.size());; 
    rep.headers[1].first = "Content-Type";
    rep.headers[1].second = "text/html"; 
    //rep = reply::stock_reply(reply::not_found);
    return;
  }


  // Fill out the reply to be sent to the client.
  std::cout << "File at path: " << full_path.c_str() << " is valid!" << "\n";
  rep.status_code = "200";
  rep.http_version_ = "1.0";
  rep.reason_phrase_ = "OK";
  char buf[512];
  while (is.read(buf, sizeof(buf)).gcount() > 0)
  rep.body_.append(buf, is.gcount());
  rep.headers.resize(2);
  rep.headers[0].first = "Content-Length";
  rep.headers[0].second = boost::lexical_cast<std::string>(rep.body_.size());
  rep.headers[1].first = "Content-Type";
  rep.headers[1].second = mime_types::extension_to_type(extension);
}

bool request_handler::url_decode(const std::string& in, std::string& out)
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
}

} // namespace server
} // namespace http