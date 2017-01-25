#ifndef WEBSERVER_H
#define	WEBSERVER_H

#include <boost/asio.hpp>
#include <boost/regex.hpp>

#include <unordered_map>
#include <thread>

using namespace std;
using namespace boost::asio;

struct Request {
    string method, path, http_version;
    
    shared_ptr<istream> content;
    
    unordered_map<string, string> headers;
};

class WebServer {
public:
    unordered_map<string, unordered_map<string, function<void(ostream&, const Request&, const boost::smatch&)> > > resources;
    
    WebServer(unsigned short, size_t);
    
    void run();
            
private:
    io_service m_io_service;
    ip::tcp::endpoint endpoint;
    ip::tcp::acceptor acceptor;
    size_t num_threads;
    vector<thread> threads;

    void do_accept();
    
    void process_request_and_respond(shared_ptr<ip::tcp::socket> socket);
    
    Request parse_request(istream& stream);
    
    void do_reply(shared_ptr<ip::tcp::socket> socket, shared_ptr<Request> request);
};

#endif	/* WEBSERVER_H */
