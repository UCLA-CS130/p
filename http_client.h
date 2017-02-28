#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include "response.h"
#include <iterator>
#include <exception>

class HTTPClient {
public:
    HTTPClient();
    // resolve and connect
    bool Connect(std::string host, std::string port = "80");
    bool SendRequest(std::string request);
    std::unique_ptr<Response> GetResponse();
    ~HTTPClient();
private:
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::socket socket_;
};

#endif
