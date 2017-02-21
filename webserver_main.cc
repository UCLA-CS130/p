#include "webserver.h"
#include <stdlib.h>
#include <map>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    //Use NginxConfigParser class to parse config file to get the port number
    Log::instance();
    NginxConfigParser parser_;
    NginxConfig config;
    parser_.Parse(argv[1], &config); 
    unsigned short port=8080;
    extract_port(config, port);
  
    WebServer webserver(config, port, 1);
    
    //Start HTTP-server
    webserver.run();
    
    return 0;
}
