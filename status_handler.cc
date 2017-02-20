#include "status_handler.h"

using namespace std;

StatusHandler::Status StatusHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
	return Status(0);
}

StatusHandler::Status StatusHandler::HandleRequest(const Request& request, Response* response) {
	return Status(0);
}
