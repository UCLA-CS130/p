#ifndef response_h
#define response_h

#include <string>
#include <vector>

class Response {
public:
	enum ResponseCode {
    	// Define your HTTP response codes here.
    	OK = 200
	};

	void SetStatus(const ResponseCode response_code);
	void AddHeader(const std::string& header_name, const std::string& header_value);
	void SetBody(const std::string& body);

	std::string ToString();

private:
	std::string self_response_code;
	std::vector<std::pair<std::string, std::string>> self_headers;
	std::string self_body;
};


#endif /* response_h */