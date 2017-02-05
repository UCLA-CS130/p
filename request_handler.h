#ifndef request_handler_h
#define request_handler_h

#include <boost/regex.hpp>

#include <vector>

#include "request.h"

using namespace std;

class RequestHandler {
public:
    RequestHandler(shared_ptr<vector<string>> paths): paths(paths) {}
    
    virtual void get_response(ostream& response, const Request&) = 0;
    
    shared_ptr<vector<string>> paths;
};

#endif /* request_handler_h */
