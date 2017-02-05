#include "webserver.h"
#include "config_parser.h"
#include <stdlib.h>
#include <map>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    //Use NginxConfigParser class to parse config file to get the port number
    NginxConfigParser parser_;
    NginxConfig out_config_;
    parser_.Parse(argv[1], &out_config_); 
    string port = out_config_.statements_[0]->child_block_->statements_[0]->tokens_[1];
    unsigned short port_num = (unsigned short) atoi(port.c_str());

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
    auto paths1 = make_shared<unordered_set<string>>();
    paths1->insert("/");
    auto p1 = make_shared<RequestHandlerEcho>(paths1);

    auto paths2 = make_shared<unordered_map<string, string>>();
    auto p2 = make_shared<RequestHandlerStatic>(paths2); 
    WebServer webserver(port_num, p1, p2, 1);
    
    //Start HTTP-server
    webserver.run();
    
    return 0;
}
