#include "webserver.h"
#include <cstring>

WebServer::WebServer(NginxConfig config, unsigned short port, size_t num_threads=4) 
    : endpoint(ip::tcp::v4(), port), acceptor(m_io_service, endpoint), num_threads(num_threads)
    { extract(config); }

void WebServer::run() {
    do_accept();

    //If num_threads>1, start m_io_service.run() in (num_threads-1) threads for thread-pooling
    for(size_t c=1;c<num_threads;c++) {
        threads.emplace_back([this](){ m_io_service.run();});
    }

    //Main thread
    m_io_service.run();

    //Wait for the rest of the threads, if any, to finish as well
    for(thread& t: threads) { t.join(); }
}

void WebServer::do_accept() {
    //Create new socket for this connection
    //Shared_ptr is used to pass temporary objects to the asynchronous functions
    shared_ptr<ip::tcp::socket> socket(new ip::tcp::socket(m_io_service));

    acceptor.async_accept(*socket, [this, socket](const boost::system::error_code& ec) {
        //Immediately start accepting a new connection
        do_accept();

        if(!ec) { process_request(socket); }
    });
}

void WebServer::process_request(shared_ptr<ip::tcp::socket> socket) {
    //Create new read_buffer for async_read_until()
    //Shared_ptr is used to pass temporary objects to the asynchronous functions
    shared_ptr<boost::asio::streambuf> read_buffer(new boost::asio::streambuf);
    async_read_until(*socket, *read_buffer, "\r\n\r\n",
    [this, socket, read_buffer](const boost::system::error_code& ec, size_t bytes_transferred) {
        if(!ec) {
            //read_buffer->size() is not necessarily the same as bytes_transferred, from Boost-docs:
            //"After a successful async_read_until operation, the streambuf may contain additional data beyond the delimiter"
            //The chosen solution is to extract lines from the stream directly when parsing the header. What is left of the
            //read_buffer (maybe some bytes of the content) is appended to in the async_read-function below (for retrieving content).
            size_t total=read_buffer->size();
            size_t num_additional_bytes=total-bytes_transferred;

            string raw_request((istreambuf_iterator<char>(read_buffer.get())), istreambuf_iterator<char>());
            unique_ptr<Request> request = Request::Parse(raw_request);
            auto headers = request->headers();

            //If content, read that as well
            auto it = find_if(headers.begin(), headers.end(), [](const std::pair<string, string>& header) { 
                return header.first == "Content-Length"; 
            } );
            if(it != headers.end()) {
                async_read(*socket, *read_buffer, transfer_exactly(stoull(it->second)-num_additional_bytes), 
                [this, socket, read_buffer, &request](const boost::system::error_code& ec, size_t bytes_transferred) {
                    if(!ec) {

                        string body((istreambuf_iterator<char>(read_buffer.get())), istreambuf_iterator<char>());
                        request->setBody(body);

                        do_reply(socket, request);
                    }
                });
            }
            else { do_reply(socket, request); }
        }
    });
}

void WebServer::do_reply(shared_ptr<ip::tcp::socket> socket, const unique_ptr<Request> &request) {
    shared_ptr<boost::asio::streambuf> write_buffer(new boost::asio::streambuf);
    ostream response(write_buffer.get());

    // Use longest prefix matching to map to corresponding request handler
    string uri = request->uri();
    size_t pos;
    shared_ptr<RequestHandler> handler = nullptr;
    string prefix;
    while ((pos = uri.find_last_of("/")) != string::npos) {
        
        if(pos == 0){
          prefix = uri.substr(0, pos+1);
        }
        else{
          prefix = uri.substr(0, pos);
        }

        auto it = prefix2handler.find(prefix);
        if (it != prefix2handler.end()) {
            handler = prefix2handler[prefix];
            break;
        }
        uri = prefix;
    }

    Response res;
    // Response_code (200, 404, etc) will always appear after "HTTP/1.1 ".
    // So get the const size of http version and the const length of response_code.
    // Then, response_code can be obtained from substr of response.ToString() function
    // and be written to singleton Log class.
    const int http_version_size = strlen("HTTP/1.1 ");
    const int response_code_len = 3;
    if (handler) {
        handler->HandleRequest(*request, &res);
        // Write status information of the server to singleton Log class.
        Log::instance()->set_status(request->uri(), res.ToString().substr(http_version_size, response_code_len), prefix2handler_type[prefix], prefix);
    }
    else{
        prefix2handler["default"]->HandleRequest(*request, &res);
        // Write status information of the server to singleton Log class.
        Log::instance()->set_status(request->uri(), res.ToString().substr(http_version_size, response_code_len), "NotFoundHandler", "");
    }

    response << res.ToString();

    int version = stoi(request->version());
    // Capture write_buffer in lambda so it is not destroyed before async_write is finished
    async_write(*socket, *write_buffer, [this, socket, write_buffer, version](const boost::system::error_code& ec, size_t bytes_transferred) {
        //HTTP persistent connection (HTTP 1.1):
        if(!ec && version>=1.1) {
            process_request(socket);
        }
    });
    return;
}

void extract_port(NginxConfig config, unsigned short &port) {
  // Initialize variables
  string key = "";
  string value = "";

  for (size_t i = 0; i < config.statements_.size(); i++) {
    //search in child block
    if (config.statements_[i]->child_block_ != nullptr) {
        extract_port(*(config.statements_[i]->child_block_), port);
    }

    if (config.statements_[i]->tokens_.size() >= 1) {
      key = config.statements_[i]->tokens_[0];
    }

    if (config.statements_[i]->tokens_.size() >= 2) {
      value = config.statements_[i]->tokens_[1];
    }

    if (key == "port" && value != "") {
      port = atoi(value.c_str());
    }
  }
}

void WebServer::extract(NginxConfig config) {
  // Initialize variables
  string key = "";
  string value = "";

  for (size_t i = 0; i < config.statements_.size(); i++) {
    //search in child block
    if (config.statements_[i]->child_block_ != nullptr) {
      // create corresponding request handler objects
      if(config.statements_[i]->tokens_[0] == "path"){
        key = config.statements_[i]->tokens_[1];
        string handler_type = config.statements_[i]->tokens_[2];

        // create a request handler pointer by handler_type
        auto handler = RequestHandler::CreateByName(handler_type.c_str());
        // and then assign it to the corresponding shared pointer in prefix2handler map
        prefix2handler[key].reset(handler);

        prefix2handler[key]->Init(key, *(config.statements_[i]->child_block_));

        // record handler type for logging status of the server
        prefix2handler_type[key] = handler_type;

      }
      else if(config.statements_[i]->tokens_[0] == "default"){
        string handler_type = config.statements_[i]->tokens_[1];
        auto handler = RequestHandler::CreateByName(handler_type.c_str());
        prefix2handler["default"].reset(handler);
      }
      else{
        extract(*(config.statements_[i]->child_block_));
      }
    }
  }
}
