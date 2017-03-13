#include "webserver_https.h"
#include <stdlib.h>
#include <map>
#include <iostream>

int main(int argc, char* argv[]) {
    //Use NginxConfigParser class to parse config file to get the port number
    Log::instance();
    NginxConfigParser parser_;
    NginxConfig config;
    parser_.Parse(argv[1], &config); 
    unsigned short port=8080;
    extract_port(config, port);
  
    WebServerHTTPS webserver(config, port, 4, "server.crt", "server.key");
    
    //Start HTTP-server
    webserver.run();
    
    return 0;
}
