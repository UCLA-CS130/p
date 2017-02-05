#ifndef request_handler_static_h
#define request_handler_static_h

#include "request_handler.h"

class RequestHandlerStatic : public RequestHandler {
public:
	RequestHandlerStatic (shared_ptr<vector<string>> paths) : RequestHandler(paths) {}

    void get_response(ostream& response, const Request&, const boost::smatch&) {
        string filename="static/";      
        string path=path_match[1];
        
        //Remove all but the last '.' (so we can't leave the web-directory)
        size_t last_pos=path.rfind(".");
        size_t current_pos=0;
        size_t pos;
        while((pos=path.find('.', current_pos))!=string::npos && pos!=last_pos) {
            cout << pos << endl;
            current_pos=pos;
            path.erase(pos, 1);
            last_pos--;
        }
        
        filename+=path;
        ifstream ifs;
        //A simple platform-independent file-or-directory check do not exist, but this works in most of the cases:
        if(filename.find('.')==string::npos) {
            if(filename[filename.length()-1]!='/')
                filename+='/';
            filename+="index.html";
        }
        ifs.open(filename, ifstream::in);
        if(ifs) {
            ifs.seekg(0, ios::end);
            size_t length=ifs.tellg();
            
            ifs.seekg(0, ios::beg);

            //The file-content is copied to the response-stream. Should not be used for very large files.
            response << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n\r\n" << ifs.rdbuf();

            ifs.close();
        }
        else {
            string content="Could not open file "+filename;
            response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
        }
    };
    
    }
};

#endif /* request_handler_static_h */
