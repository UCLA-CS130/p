#ifndef request_handler_h
#define request_handler_h

#include "request.h"

using namespace std;

class RequestHandler {
public:
	RequestHandler() {}
	
    virtual void get_response(string& response, const Request&) = 0;
};

#endif /* request_handler_h */
