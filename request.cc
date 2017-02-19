#include "request.h"

#include <fstream>
#include <boost/regex.hpp>

using namespace std;

unique_ptr<Request> Request::Parse(const std::string& raw_request) {
	unique_ptr<Request> request(new Request(raw_request));

    boost::regex e("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    boost::smatch sm;

    istringstream stream(raw_request);

    //First parse request method, path, and HTTP-version from the first line
    string line;
    getline(stream, line);
    line.pop_back();

    if(regex_match(line, sm, e)) {        
        request->m_method=sm[1];
        request->m_uri=sm[2];
        request->m_version=sm[3];

        bool matched;
        e="^([^:]*): ?(.*)$";
        //Parse the rest of the header
        do {
            getline(stream, line);
            line.pop_back();
            matched=regex_match(line, sm, e);
            if(matched) {
                request->m_headers.push_back({sm[1],sm[2]});
            }

        } while(matched==true);
    }

    return request;
}

std::string Request::raw_request() const { return m_raw_request; }

std::string Request::method() const { return m_method; }

std::string Request::uri() const { return m_uri; }

std::string Request::version() const { return m_version; }

Request::Headers Request::headers() const { return m_headers; }



