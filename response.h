#ifndef response_h
#define response_h

#include <string>
#include <vector>
#include <memory>

class Response {
public:
	enum ResponseCode {
    	// Define your HTTP response codes here.
    	OK = 200,
        MOVE_PERMANENTLY = 301,
        MOVE_TEMPORARILY = 302,
    	NOT_FOUND = 404
	};

    static std::unique_ptr<Response> ParseHeader(std::string header);

	void SetStatus(const ResponseCode response_code);
	void AddHeader(const std::string& header_name, const std::string& header_value);
	void SetBody(const std::string& body);

	std::string ToString();

    std::vector<std::pair<std::string, std::string>> GetHeaders();
    ResponseCode GetResponseCode();
    std::string GetBody();

private:
	std::string self_response_code;
	std::vector<std::pair<std::string, std::string>> self_headers;
	std::string self_body;
    ResponseCode  response_code_;

    static char space_ ;
    static std::string header_separator_;
};


#endif /* response_h */
