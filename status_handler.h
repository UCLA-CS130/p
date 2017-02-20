#ifndef status_handler_h
#define status_handler_h

#include "request_handler.h"

class StatusHandler : public RequestHandler {
public:
	Status Init(const std::string& uri_prefix, const NginxConfig& config);

	Status HandleRequest(const Request& request, Response* response);
};

#endif /* status_handler_h */
