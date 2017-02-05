#ifndef request_handler_static_h
#define request_handler_static_h

#include "request_handler.h"

#include <unordered_map>

class RequestHandlerStatic : public RequestHandler {
public:
	RequestHandlerStatic (shared_ptr<unordered_map<string, string>> paths) : paths(paths) {}

    void get_response(ostream& response, const Request&) {
	
    }

    shared_ptr<unordered_map<string, string>> paths;
};

#endif /* request_handler_static_h */
