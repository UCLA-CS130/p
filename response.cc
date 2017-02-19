#include "response.h"

void Response::SetStatus(const ResponseCode response_code)
{
	switch(response_code){
		case OK: default: self_response_code = "200 OK"; break;
	}
}

void Response::AddHeader(const std::string& header_name, const std::string& header_value)
{
	self_headers[header_name] = header_value;
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