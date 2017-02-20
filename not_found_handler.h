#ifndef not_found_handler_h
#define not_found_handler_h

#include "request_handler.h"

class NotFoundHandler : public RequestHandler {
public:
	Status Init(const std::string& uri_prefix, const NginxConfig& config);

	Status HandleRequest(const Request& request, Response* response);
};

#endif /* not_found_handler_h */
