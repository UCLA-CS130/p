#ifndef echo_handler_h
#define echo_handler_h

#include "request_handler.h"

class EchoHandler : public RequestHandler {
public:
	Status Init(const std::string& uri_prefix, const NginxConfig& config);

	Status HandleRequest(const Request& request, Response* response);
};

#endif /* echo_handler_h */
