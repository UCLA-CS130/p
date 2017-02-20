#ifndef WEBSERVER_H
#define	WEBSERVER_H

#include <boost/asio.hpp>
#include <boost/regex.hpp>

#include <thread>
#include <unordered_map>

//#include "config_parser.h"
#include "request.h"
#include "response.h"
#include "request_handler.h"
#include "echo_handler.h"
#include "static_handler.h"

using namespace std;
using namespace boost::asio;

class WebServer {
public:
    WebServer(NginxConfig config, unsigned short port, size_t);
    
    void run();
            
private:
    io_service m_io_service;
    ip::tcp::endpoint endpoint;
    ip::tcp::acceptor acceptor;
    size_t num_threads;
    vector<thread> threads;

    unordered_map<string, shared_ptr<RequestHandler>> prefix2handler;

    void do_accept();
    
    void process_request(shared_ptr<ip::tcp::socket> socket);
    
    void do_reply(shared_ptr<ip::tcp::socket> socket, const unique_ptr<Request> &request);

    void extract(NginxConfig config);
};

void extract_port(NginxConfig config, unsigned short& port);

#endif	/* WEBSERVER_H */
