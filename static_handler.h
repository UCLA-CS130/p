#ifndef static_handler_h
#define static_handler_h

#include "request_handler.h"

class StaticHandler : public RequestHandler {
public:
	Status Init(const std::string& uri_prefix, const NginxConfig& config);

	Status HandleRequest(const Request& request, Response* response);

private:
	std::string m_uri_prefix, m_root;
};

#endif /* static_handler_h */
