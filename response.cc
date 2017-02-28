#include "response.h"

char Response::space_ = ' ';
std::string Response::header_separator_ = "\r\n";

void Response::SetStatus(const ResponseCode response_code)
{
	switch(response_code){
		case NOT_FOUND: self_response_code = "404 NOT FOUND"; break;
		case OK: default: self_response_code = "200 OK"; break;
	}
	response_code_ = response_code;
}

void Response::AddHeader(const std::string& header_name, const std::string& header_value)
{
	self_headers.push_back({header_name, header_value});
}

void Response::SetBody(const std::string& body)
{
	self_body = body;
}

std::string Response::ToString(){
	std::string res;
	res += "HTTP/1.1 ";
	res += self_response_code;
	res += "\r\n";

	for(auto header:self_headers){
		res += header.first;
		res += ": ";
		res += header.second;
		res += "\r\n";
	}
	res += "\r\n";

	res += self_body;

	return res;
}

std::unique_ptr<Response> Response::ParseHeader(std::string header) {
    auto response = std::unique_ptr<Response>(new Response());
    size_t n = 0, prev = 0;

    n = header.find("\r\n");
    std::string status_line = header.substr(0, n);
    header = header.substr(n+2);

    // parse status line
    n = status_line.find(space_, prev);
    prev = n + 1;
    n = status_line.find(space_, prev);
    response->SetStatus(static_cast<ResponseCode >(std::stoi(status_line.substr(prev, n-prev))));

    // parse headers
    prev = 0;
    while ((n = header.find(header_separator_, prev)) != std::string::npos) {
        std::string line = header.substr(prev, n-prev);
        if (line.length() == 0) break;
        prev = n + header_separator_.length();
        size_t  mid = line.find(":");
        std::string name = line.substr(0, mid);
        std::string value = line.substr(mid+1);
        value = value.substr(value.find_first_not_of(' '));
        response->AddHeader(name, value);
    }

    return response;
}

std::vector<std::pair<std::string, std::string>> Response::GetHeaders() {
    return self_headers;
}

Response::ResponseCode Response::GetResponseCode() {
    return response_code_;
}

std::string Response::GetBody() {
    return self_body;
}
