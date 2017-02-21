#ifndef status_handler_h
#define status_handler_h

#include "request_handler.h"
#include <vector>

class StatusHandler : public RequestHandler {
private:
	struct status{
		std::string s_url;
		std::string s_response_code;
		std::string s_request_handler;
		std::string s_url_prefix;
	};

	std::vector<status> statuses;

public:
	Status Init(const std::string& uri_prefix, const NginxConfig& config);

	Status HandleRequest(const Request& request, Response* response);

	Status Log(const std::string& url, const std::string& response_code, const std::string& handler, const std::string& url_prefix);
};

#endif /* status_handler_h */
