#include "webserver.h"
#include "config_parser.h"
#include <stdlib.h>

//Added for the json-example:
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
//Added for the json-example:
using namespace boost::property_tree;

int main(int argc, char* argv[]) {
    //Use NginxConfigParser class to parse config file to get the port number
    NginxConfigParser parser_;
    NginxConfig out_config_;
    parser_.Parse(argv[1], &out_config_);
    string port = out_config_.statements_[0]->child_block_->statements_[0]->tokens_[1];
    unsigned short port_num = (unsigned short) atoi(port.c_str());

    //HTTP-server at port specified in config file using 4 threads
    WebServer webserver(port_num, 4);
    
    //Add resources using regular expression for path, a method-string, and an anonymous function
    //POST-example for the path /string, responds the posted string
    webserver.resources["^/string/?$"]["POST"]=[](ostream& response, const Request& request, const boost::smatch& path_match) {
        //Retrieve string from istream (*request.content)
        stringstream ss;
        *request.content >> ss.rdbuf();
        string content=move(ss.str());
        
        response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    };
    
    //POST-example for the path /json, responds firstName+" "+lastName from the posted json
    //Responds with an appropriate error message if the posted json is not valid, or if firstName or lastName is missing
    //Example posted json:
    //{
    //  "firstName": "John",
    //  "lastName": "Smith",
    //  "age": 25
    //}
    webserver.resources["^/json/?$"]["POST"]=[](ostream& response, const Request& request, const boost::smatch& path_match) {
        try {
            ptree pt;
            read_json(*request.content, pt);

            string name=pt.get<string>("firstName")+" "+pt.get<string>("lastName");

            response << "HTTP/1.1 200 OK\r\nContent-Length: " << name.length() << "\r\n\r\n" << name;
        }
        catch(exception& e) {
            response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n" << e.what();
        }
    };
    
    //GET-example for the path /
    //Responds with request-information
    webserver.resources["^/$"]["GET"]=[](ostream& response, const Request& request, const boost::smatch& path_match) {
        stringstream content_stream;
        content_stream << request.method << " " << request.path << " HTTP/" << request.http_version << "\r\n";
        for(auto& header: request.header) {
            content_stream << header.first << ": " << header.second << "\r\n";
        }
        
        //find length of content_stream (length received using content_stream.tellp())
        content_stream.seekp(0, ios::end);
        
        response <<  "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\nContent-Type: text/plain\r\n\r\n" << content_stream.rdbuf();
    };
    
    //GET-example for the path /match/[number], responds with the matched string in path (number)
    //For instance a request GET /match/123 will receive: 123
    webserver.resources["^/match/([0-9]+)/?$"]["GET"]=[](ostream& response, const Request& request, const boost::smatch& path_match) {
        string number=path_match[1];
        response << "HTTP/1.1 200 OK\r\nContent-Length: " << number.length() << "\r\n\r\n" << number;
    };
    
    //Start HTTP-server
    webserver.start();
    
    return 0;
}