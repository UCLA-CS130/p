#include "not_found_handler.h"

using namespace std;

NotFoundHandler::Status NotFoundHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
	return Status(0);
}

NotFoundHandler::Status NotFoundHandler::HandleRequest(const Request& request, Response* response) {
	response->SetStatus(Response::ResponseCode(404));
	string content="404 Not Found.";
	response->AddHeader("Content-Length", to_string(content.size()));
	response->AddHeader("Content-Type", "text/plain"); 
	response->SetBody(content);
	return Status(0);
}
