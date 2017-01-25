#include "webserver.h"

using namespace std;

int main() {
    //HTTP-server at port 8080 using 4 threads
    WebServer webserver(8080, 4);
    
    //GET-example for the path /
    //Responds with request-information
    webserver.resources["^/$"]["GET"]=[](ostream& response, const Request& request, const boost::smatch& path_match) {
        stringstream content_stream;
        content_stream << "<h1>Request:</h1>";
        content_stream << request.method << " " << request.path << " HTTP/" << request.http_version << "<br>";
        for(auto& header: request.headers) {
            content_stream << header.first << ": " << header.second << "<br>";
        }
        
        //find length of content_stream (length received using content_stream.tellp())
        content_stream.seekp(0, ios::end);
        
        response <<  "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
    };
    
    //Start HTTP-server
    webserver.run();
    
    return 0;
}