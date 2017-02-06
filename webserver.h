#ifndef WEBSERVER_H
#define	WEBSERVER_H

#include <boost/asio.hpp>
#include <boost/regex.hpp>

#include <thread>

#include "request.h"
#include "request_handler.h"
#include "request_handler_echo.h"
#include "request_handler_static.h"
#include "config_parser.h"

using namespace std;
using namespace boost::asio;

class WebServer {
public:
    WebServer(NginxConfig config, unsigned short port, size_t);
    
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

    void extract(NginxConfig config);

    void extract_location(NginxConfig config, string path);
};

void extract_port(NginxConfig config, unsigned short& port);

#endif	/* WEBSERVER_H */
