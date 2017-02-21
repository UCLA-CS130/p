#ifndef request_handler_h
#define request_handler_h

#include "request.h"
#include "log.h"
#include "response.h"
#include "config_parser.h"

//class NginxConfig;

class RequestHandler {
public:
	enum Status {
		OK = 0, 
		ILLEGAL_CONFIG = 1
	};

	virtual Status Init(const std::string& uri_prefix, const NginxConfig& config) = 0;

	virtual Status HandleRequest(const Request& request, Response* response) = 0;

	// add this function for status handler
	// virtual Status Log(const std::string& url, const std::string& response_code, const std::string& handler, const std::string& url_prefix){
	// 	return Status(0);
	// }
};

#endif /* request_handler_h */
