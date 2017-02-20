#include "echo_handler.h"

using namespace std;

EchoHandler::Status EchoHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {

	return Status(0);
}

EchoHandler::Status EchoHandler::HandleRequest(const Request& request, Response* response) {
	response->SetStatus(Response::ResponseCode(200));
	response->AddHeader("Content-Length", to_string(request.raw_request().size()));
	response->AddHeader("Content-Type", "text/plain"); 
	response->SetBody(request.raw_request());
	return Status(0);
}
