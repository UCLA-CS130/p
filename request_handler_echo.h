#ifndef request_handler_echo_h
#define request_handler_echo_h

#include "request_handler.h"

#include <unordered_set>

class RequestHandlerEcho : public RequestHandler {
public:
    RequestHandlerEcho (shared_ptr<unordered_set<string>> paths) : paths(paths) {}

    void get_response(string& response, const Request& request);

    shared_ptr<unordered_set<string>> paths;
};

#endif /* request_handler_echo_h */
