#ifndef PROXY_HANDLER_H
#define PROXY_HANDLER_H

#include "request_handler.h"
#include "http_client.h"
#include "UriParser.hpp"
#include <string>
#include <sstream>
#include <iostream>

class ProxyHandler : public RequestHandler {
public:
    //NotFoundHandler(const std::string& reason) : reason_response(reason) {}
    // Initializes the handler. Returns a response code indicating success or
    // failure condition.
    // uri_prefix is the value in the config file that this handler will run for.
    // config is the contents of the child block for this handler ONLY.
    virtual Status Init(const std::string& uri_prefix,
                  const NginxConfig& config);

    // Handles an HTTP request, and generates a response. Returns a response code
    // indicating success or failure condition. If ResponseCode is not OK, the
    // contents of the response object are undefined, and the server will return
    // HTTP code 500.
    virtual Status HandleRequest(const Request& request,
                           Response* response);
private:
    std::string host_;
    std::string port_ = "80";

    static std::string filter_request_header(const Request& req);
    static std::string request_from_url(const std::string url);
};

REGISTER_REQUEST_HANDLER(ProxyHandler);

#endif
