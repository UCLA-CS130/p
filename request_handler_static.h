#ifndef request_handler_static_h
#define request_handler_static_h

#include "request_handler.h"

class RequestHandlerStatic : public RequestHandler {
public:
	RequestHandlerStatic (shared_ptr<vector<string>> paths) : RequestHandler(paths) {}

    void get_response(ostream& response, const Request&) {
        
    }
};

#endif /* request_handler_static_h */
