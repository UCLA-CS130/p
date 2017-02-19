#include "webserver.h"

void RequestHandlerEcho::get_response(string& response, const Request& request) {
    // stringstream content_stream;
    // content_stream << request.method << " " << request.path << " HTTP/" << request.http_version << "\r\n";
    // map<string, string> ordered(request.headers.begin(), request.headers.end());
    // for(auto rit=ordered.rbegin(); rit!=ordered.rend(); ++rit) {
    //     content_stream << rit->first << ": " << rit->second << "\r\n";
    // }
    // content_stream << "\r\n";

    //     //find length of content_stream (length received using content_stream.tellp())
    // content_stream.seekp(0, ios::end);

    // response <<  "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\nContent-Type: text/plain\r\n\r\n" << content_stream.rdbuf();
}

void RequestHandlerStatic::get_response(string& response, const Request& request) {

    // string p=request.path.substr(1);
    // size_t position=p.find_first_of("/");
    // string key;
    // if(position == string::npos){
    //     key = p;
    // }
    // else{
    //     key = p.substr(0, position);
    // }
    // auto find_base_path = paths->find(key);

    //     // if paths not found, return 404
    // if(find_base_path == paths->end()){
    //     string content="Could not find file "+p;
    //     response << "HTTP/1.1 404 Not Found\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    //     return;
    // }
    // string filename = find_base_path->second;

    // // initialize varibles before goto
    // string path;
    // string content_type;
    // string type;
    // size_t current_pos=0;
    // size_t last_pos=0;

    // if(position == string::npos){
    //     goto index_page;
    // }

    // path = p.substr(position);

    //     // Remove all but the last '.' (so we can't leave the web-directory)
    // last_pos=path.rfind(".");
    // size_t pos;
    // while((pos=path.find('.', current_pos))!=string::npos && pos!=last_pos) {
    //     current_pos=pos;
    //     path.erase(pos, 1);
    //     last_pos--;
    // }

    //     // form content type field
    //     // only covers common types since this is a simple webserver
    // content_type = "";
    // type = path.substr(last_pos+1);

    //     // image
    // if(type == "jpg" || type == "jpeg" || type == "png" || type == "gif"){
    //     content_type += "image/";
    //     content_type += type;
    // }
    //     // text
    // else if(type == "html" || type == "css" || type == "txt"){
    //     content_type += "text/";
    //     if(type == "txt"){
    //         content_type += "plain";
    //     }
    //     else{
    //         content_type += type;
    //     }
    // }
    //     // audio
    // else if(type == "mp3" || type == "wma" || type == "wav"){
    //     content_type += "audio";
    // }
    //     // video
    // else if(type == "mp4" || type == "mov" || type == "wmv"){
    //     content_type += "video";
    // }
    //     // application
    // else if(type == "pdf" || type == "xml" || type == "zip"){
    //     content_type += "application/";
    //     content_type += type;
    // }

    // index_page:
    // filename+=path;
    // ifstream ifs;
    //     //A simple platform-independent file-or-directory check do not exist, but this works in most of the cases:
    // if(filename.find('.')==string::npos) {
    //     if(filename[filename.length()-1]!='/')
    //         filename+='/';
    //     filename+="index.html";
    //     content_type = "text/html";
    // }
    // ifs.open(filename, ifstream::in);
    // if(ifs) {
    //     ifs.seekg(0, ios::end);
    //     size_t length=ifs.tellg();

    //     ifs.seekg(0, ios::beg);

    //         //The file-content is copied to the response-stream. Should not be used for very large files.
    //     response << "HTTP/1.1 200 OK\r\nContent-Length: " << length <<"\r\nContent-Type: "<<content_type<<"\r\n\r\n" << ifs.rdbuf();

    //     ifs.close();
    // }
    // else {
    //     string content="Could not find file "+filename;
    //     response << "HTTP/1.1 404 Not Found\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    // }
}

WebServer::WebServer(NginxConfig config, unsigned short port, size_t num_threads=1) 
    : endpoint(ip::tcp::v4(), port), acceptor(m_io_service, endpoint), num_threads(num_threads)
    {
        // initialize echo hadler
        auto paths1 = make_shared<unordered_set<string>>();
        echo_handler = make_shared<RequestHandlerEcho>(paths1);
        echo_handler->paths->insert("/");

        // initialize static handler
        auto paths2 = make_shared<unordered_map<string, string>>();
        static_handler = make_shared<RequestHandlerStatic>(paths2); 

        extract(config);
    }

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
            // size_t total=read_buffer->size();

            //Convert to istream to extract string-lines
            istream stream(read_buffer.get());
            string raw_request((istreambuf_iterator<char>(read_buffer.get())), istreambuf_iterator<char>());
            unique_ptr<Request> request = Request::Parse(raw_request);

            cout << "raw: " << request->raw_request() << endl;
            cout << "method: " << request->method() << endl;
            cout << "uri: " << request->uri() << endl;
            cout << "version: " << request->version() << endl << "headers: ";
            auto headers = request->headers();
            for (auto &header : headers)
                cout << header.first << ": " << header.second << endl;
            // size_t num_additional_bytes=total-bytes_transferred;

            // //If content, read that as well
            // if(request->headers.count("Content-Length")>0) {
            //     async_read(*socket, *read_buffer, transfer_exactly(stoull(request->headers["Content-Length"])-num_additional_bytes), 
            //     [this, socket, read_buffer, request](const boost::system::error_code& ec, size_t bytes_transferred) {
            //         if(!ec) {
            //             //Store pointer to read_buffer as istream object
            //             request->content=shared_ptr<istream>(new istream(read_buffer.get()));

            //             do_reply(socket, request);
            //         }
            //     });
            // }
            // else { do_reply(socket, request);}
        }
    });
}

void WebServer::do_reply(shared_ptr<ip::tcp::socket> socket, shared_ptr<Request> request) {
    //Find path- and method-match, and generate response
    shared_ptr<boost::asio::streambuf> write_buffer(new boost::asio::streambuf);
    //ostream response(write_buffer.get());
    string response((istreambuf_iterator<char>(write_buffer.get())), istreambuf_iterator<char>());
    if (echo_handler->paths->find(request->path) != echo_handler->paths->end()) {
        echo_handler->get_response(response, *request);
    }
    else {
        static_handler->get_response(response, *request);
    }
    //Capture write_buffer in lambda so it is not destroyed before async_write is finished
    async_write(*socket, *write_buffer, [this, socket, request, write_buffer](const boost::system::error_code& ec, size_t bytes_transferred) {
        //HTTP persistent connection (HTTP 1.1):
        if(!ec && stof(request->http_version)>1.05)
            process_request(socket);
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

    if (key == "listen" && value != "") {
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
      if(config.statements_[i]->tokens_[0] == "location"){
        extract_location(*(config.statements_[i]->child_block_), config.statements_[i]->tokens_[1]);
      }
      else{
        extract(*(config.statements_[i]->child_block_));
      }
    }
  }
}

void WebServer::extract_location(NginxConfig config, string path){
    // Initialize variables
    string key = "";
    string value = "";

    for (size_t i=0; i < config.statements_.size(); i++){
      if (config.statements_[i]->child_block_ != nullptr) {
            extract_location(*(config.statements_[i]->child_block_), path);
      }

      if (config.statements_[i]->tokens_.size() >= 1) {
          key = config.statements_[i]->tokens_[0];
      }

      if (config.statements_[i]->tokens_.size() >= 2) {
          value = config.statements_[i]->tokens_[1];
      }

      if (key == "root" && value != "") {
          if(path == "echo"){
            echo_handler->paths->insert(value);
          }
          else{
            (*(static_handler->paths))[path] = value;
          }
      }
    }
}