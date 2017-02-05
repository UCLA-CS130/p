#ifndef request_handler_echo_h
#define request_handler_echo_h

#include "request_handler.h"

#include <unordered_set>

class RequestHandlerEcho : public RequestHandler {
public:
    RequestHandlerEcho (shared_ptr<unordered_set<string>> paths) : paths(paths) {}

    void get_response(ostream& response, const Request& request) {
        stringstream content_stream;
        content_stream << request.method << " " << request.path << " HTTP/" << request.http_version << "\r\n";
        map<string, string> ordered(request.headers.begin(), request.headers.end());
        for(auto rit=ordered.rbegin(); rit!=ordered.rend(); ++rit) {
            content_stream << rit->first << ": " << rit->second << "\r\n";
        }
        content_stream << "\r\n";
        
        //find length of content_stream (length received using content_stream.tellp())
        content_stream.seekp(0, ios::end);
        
        response <<  "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\nContent-Type: text/plain\r\n\r\n" << content_stream.rdbuf();
    }

    shared_ptr<unordered_set<string>> paths;
};

#endif /* request_handler_echo_h */
