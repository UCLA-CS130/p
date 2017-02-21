#include "status_handler.h"

using namespace std;

StatusHandler::Status StatusHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
	return Status(0);
}

StatusHandler::Status StatusHandler::HandleRequest(const Request& request, Response* response) {
	statuses = Log::instance()->get_statuses();
	response->SetStatus(Response::ResponseCode(200));
	string response_body;
	response_body = to_string(statuses.size()) + " requests the server has served.\r\n";
	for(auto s:statuses){
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