#ifndef WEBSERVER_H
#define	WEBSERVER_H

#include <boost/asio.hpp>
#include <boost/regex.hpp>

#include <thread>
#include <iostream>

#include "request.h"
#include "request_handler.h"
#include "request_handler_echo.h"
#include "request_handler_static.h"

using namespace std;
using namespace boost::asio;

class WebServer {
public:
    WebServer(unsigned short, shared_ptr<RequestHandlerEcho>, shared_ptr<RequestHandlerStatic>, size_t);
    
    void run();

static Request parse_request(istream& stream);
            
private:
    io_service m_io_service;
    ip::tcp::endpoint endpoint;
    ip::tcp::acceptor acceptor;
    size_t num_threads;
    vector<thread> threads;

    shared_ptr<RequestHandlerEcho> echo_handler;
    shared_ptr<RequestHandlerStatic> static_handler;

    void do_accept();
    
    void process_request(shared_ptr<ip::tcp::socket> socket);
    
    void do_reply(shared_ptr<ip::tcp::socket> socket, shared_ptr<Request> request);
};

#endif	/* WEBSERVER_H */
