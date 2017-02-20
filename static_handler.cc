#include "static_handler.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

RequestHandler::Status StaticHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
	return Status(0);
}

RequestHandler::Status StaticHandler::HandleRequest(const Request& request, Response* response) {

    string filename = m_root + request.uri().substr(m_uri_prefix.size());
    cout << filename << endl;
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

    // form content type field
    // only covers common types since this is a simple webserver
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
    // //A simple platform-independent file-or-directory check do not exist, but this works in most of the cases:
    // if(filename.find('.')==string::npos) {
    //     if(filename[filename.length()-1]!='/')
    //         filename+='/';
    //     filename+="index.html";
    //     content_type = "text/html";
    // }
    ifs.open(filename, ifstream::in);
    if(ifs) {
        ifs.seekg(0, ios::end);
        size_t length=ifs.tellg();
        ifs.seekg(0, ios::beg);

		stringstream ss;
		ss << ifs.rdbuf();	
		ifs.close();

		response->SetStatus(Response::ResponseCode(200));
		response->AddHeader("Content-Length", to_string(length));
		if (!content_type.empty())
			response->AddHeader("Content-Type", content_type); 
		response->SetBody(ss.str());
    }
    else response = nullptr;
    // else {
    //     string content="Could not find file "+filename;
    //     response << "HTTP/1.1 404 Not Found\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    // }

	return Status(0);
}
