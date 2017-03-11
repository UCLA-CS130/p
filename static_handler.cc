#include "static_handler.h"
#include "not_found_handler.h"
#include "markdown.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

#include <boost/optional.hpp>

using namespace std;

StaticHandler::Status StaticHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    if (config.statements_[0]->tokens_.size() < 2) {
    	return Status(1);
    }
    m_uri_prefix = uri_prefix;
    m_root = config.statements_[0]->tokens_[1];
    return Status(0);
}

StaticHandler::Status StaticHandler::HandleRequest(const Request& request, Response* response) {
    string filename = m_root + request.uri().substr(m_uri_prefix.size());

    string content_type, ext;
    size_t pos = filename.find_last_of(".");
    if (pos != string::npos)
	    ext = filename.substr(pos+1);

    // image
    if(ext == "jpg" || ext == "jpeg" || ext == "png" || ext == "gif"){
        content_type += "image/";
        content_type += ext;
    }
    // text
    else if(ext == "html" || ext == "css" || ext == "txt"){
        content_type += "text/";
        if(ext == "txt"){
            content_type += "plain";
        }
        else if (ext == "md"){
            content_type += "html";
        }
        else{
            content_type += ext;
        }
    }
    // audio
    else if(ext == "mp3" || ext == "wma" || ext == "wav"){
        content_type += "audio";
    }
    // video
    else if(ext == "mp4" || ext == "mov" || ext == "wmv"){
        content_type += "video";
    }
    // application
    else if(ext == "pdf" || ext == "xml" || ext == "zip"){
        content_type += "application/";
        content_type += ext;
    }

    ifstream ifs;
    ifs.open(filename, ifstream::in);
    if(ifs) {
        stringstream ss;
        size_t length;
        if (ext == "md"){
            ifs.seekg(0, ios::end);
            ifs.seekg(0, ios::beg);
            istream *in=&cin;
            in=&ifs;
            
            markdown::Document doc;
            doc.read(*in);
            // doc.writeTokens(cout);
            doc.write(ss);
            length=ss.str().size();
        }else{
            ifs.seekg(0, ios::end);
            length=ifs.tellg();
            ifs.seekg(0, ios::beg);

    		ss << ifs.rdbuf();
    		ifs.close();
        }

		response->SetStatus(Response::ResponseCode(200));
		response->AddHeader("Content-Length", to_string(length));
		if (!content_type.empty())
			response->AddHeader("Content-Type", content_type); 
		response->SetBody(ss.str());
    }
    else {
    	unique_ptr<NotFoundHandler> not_found(new NotFoundHandler());
    	not_found->HandleRequest(request, response); 
    }
	return Status(0);
}
