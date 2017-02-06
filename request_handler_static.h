#ifndef request_handler_static_h
#define request_handler_static_h

#include "request_handler.h"

#include <unordered_map>

class RequestHandlerStatic : public RequestHandler {
public:
	RequestHandlerStatic (shared_ptr<unordered_map<string, string>> paths) : paths(paths) {}
    
    void get_response(ostream& response, const Request& request) {
        // string filename="file/path0";      
        // string path=request.path;
        // cout<<"line 15"<<endl;
        string p=request.path.substr(1);
        size_t position=p.find_first_of("/");
        string key = p.substr(0, position);
        //cout<<"key is "<<key<<endl;
        auto find_base_path = paths->find(key);
        if(find_base_path == paths->end()){
            string content="Could not find file "+key;
            response << "HTTP/1.1 404 Not Found\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
            return;
        }
        string filename = find_base_path->second;
        //cout<<"base_path is "<<filename<<endl;
        //cout<<"static.h line 16 "<<position<<endl;
        string path = p.substr(position);
        
        //Remove all but the last '.' (so we can't leave the web-directory)
        size_t last_pos=path.rfind(".");
        size_t current_pos=0;
        size_t pos;
        while((pos=path.find('.', current_pos))!=string::npos && pos!=last_pos) {
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
            string content="Could not find file "+filename;
            response << "HTTP/1.1 404 Not Found\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
        }
    }
    

    shared_ptr<unordered_map<string, string>> paths;
};

#endif /* request_handler_static_h */
