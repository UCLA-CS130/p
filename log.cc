#include "log.h"

Log *Log::m_instance;

Log *Log::instance(){
	if(!m_instance){
		m_instance = new Log;
	}
	return m_instance;
}

void Log::set_status(const std::string& url, const std::string& response_code, const std::string& handler, const std::string& url_prefix){
	status s;
	s.s_url = url;
	s.s_response_code = response_code;
	s.s_request_handler = handler;
	s.s_url_prefix = url_prefix;
	statuses.push_back(s);
}

std::vector<status> Log::get_statuses() const{
	return statuses;
}