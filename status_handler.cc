#include "status_handler.h"

using namespace std;

StatusHandler::Status StatusHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
	return Status(0);
}

StatusHandler::Status StatusHandler::HandleRequest(const Request& request, Response* response) {
	response->SetStatus(Response::ResponseCode(200));
	string response_body;
	response_body = to_string(statuses.size()) + " requests the server has served.\r\n";
	for(status s:statuses){
		response_body += "\r\nRequested URL: ";
		response_body += s.s_url;
		response_body += "\r\nResponse code: ";
		response_body += s.s_response_code;
		response_body += "\r\nRequest handler: ";
		response_body += s.s_request_handler;
		response_body += "\r\nFor prefix: ";
		response_body += s.s_url_prefix;
		response_body += "\r\n";
	}
	response->AddHeader("Content-Length", to_string(response_body.size()));
	response->AddHeader("Content-Type", "text/plain");
	//cout<<"response_body is "<<response_body<<endl;
	response->SetBody(response_body);
	return Status(0);
}

StatusHandler::Status StatusHandler::Log(const std::string& url, const std::string& response_code, const std::string& handler, const std::string& url_prefix) {
	status s;
	s.s_url = url;
	s.s_response_code = response_code;
	s.s_request_handler = handler;
	s.s_url_prefix = url_prefix;
	statuses.push_back(s);
	//cout<<"response_code is "<<response_code<<endl;
	//cout<<"s.s_response_code is "<<s.s_response_code<<endl;
	return Status(0);
}