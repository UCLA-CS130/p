#ifndef status_handler_h
#define status_handler_h

#include "request_handler.h"
#include <vector>

class StatusHandler : public RequestHandler {
private:
	std::vector<status> statuses;

public:
	Status Init(const std::string& uri_prefix, const NginxConfig& config);

	Status HandleRequest(const Request& request, Response* response);
};

REGISTER_REQUEST_HANDLER(StatusHandler);

#endif /* status_handler_h */
