#include "webserver.h"
#include <stdlib.h>
#include <map>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    //Use NginxConfigParser class to parse config file to get the port number
    NginxConfigParser parser_;
    NginxConfig config;
    parser_.Parse(argv[1], &config); 
    unsigned short port = extract_port(config);
    //string port = config.statements_[0]->child_block_->statements_[0]->tokens_[1];
    //unsigned short port_num = (unsigned short) atoi(port.c_str());

  //   string property = "";
  //   string value = "";11
  //   for (size_t i = 0; i < config.statements_.size(); i++) {
  //       if (config.statements_[i]->child_block_ != nullptr) {
  //         fillOutMap(*(config.statements_[i]->child_block_));
  //     }

  //     if (config.statements_[i]->tokens_.size() >= 1) {
  //         property = config.statements_[i]->tokens_[0];
  //     }

  //     if (config.statements_[i]->tokens_.size() >= 2) {
  //         value = config.statements_[i]->tokens_[1];
  //     }

  //     if (property == "listen" && value != "") {
  //         property_to_values_["port"] = value;
  //     }
  // }

    // //GET-example for the path /
    // //Responds with request-information
    // webserver.resources["^/$"]["GET"]=[](ostream& response, const Request& request, const boost::smatch& path_match) {
    //     stringstream content_stream;
    //     content_stream << request.method << " " << request.path << " HTTP/" << request.http_version << "\r\n";
    //     map<string, string> ordered(request.headers.begin(), request.headers.end());
    //     for(auto rit=ordered.rbegin(); rit!=ordered.rend(); ++rit) {
    //         content_stream << rit->first << ": " << rit->second << "\r\n";
    //     }
    //     content_stream << "\r\n";
        
    //     //find length of content_stream (length received using content_stream.tellp())
    //     content_stream.seekp(0, ios::end);
        
    //     response <<  "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\nContent-Type: text/plain\r\n\r\n" << content_stream.rdbuf();
    // };
    // auto paths1 = make_shared<unordered_set<string>>();
    // paths1->insert("/");
    // auto p1 = make_shared<RequestHandlerEcho>(paths1);

    // auto paths2 = make_shared<unordered_map<string, string>>();
    // (*paths2)["static"] = "file/path0";
    // auto p2 = make_shared<RequestHandlerStatic>(paths2); 
    WebServer webserver(config, port, 1);
    
    //Start HTTP-server
    webserver.run();
    
    return 0;
}
