#ifndef request_h
#define request_h

#include <unordered_map>
#include <fstream>

struct Request {
    std::string method, path, http_version;
    
    std::shared_ptr<std::istream> content;
    
    std::unordered_map<std::string, std::string> headers;
};

#endif /* request_h */
