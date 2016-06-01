# 2CoolForSchool

## Where things are
- HttpRequest and HttpResponse structures in request.h/.cc and response.h/.cc
  - Helpful http constants in http_constants.h
- We have Response handler base class in request handler.h
  and sub classes in respective .cc files i.e statichandler in StaticHandler.cc
- request.cc is responsible for creating a Server and running it, it also is 
  in charge of parsing the config files into repsective handlers, and parsing
  the uri request.

## Building, testing, running
Build : `make`

Run tests: `make test`

Run server: `./wbser config`

### With docker 
Build: "docker build -t webserver ."

(has trouble running it, but builds successfuly to a small image size)
Run server: `docker run -p 8080:8080 webserver:latest > binary.tar`



