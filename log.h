#ifndef log_h
#define log_h

#include <vector>
#include <string>
#include <iostream>

struct status{
	std::string s_url;
	std::string s_response_code;
	std::string s_request_handler;
	std::string s_url_prefix;
};

class Log{
private:
	std::vector<status> statuses;

	static Log *m_instance;

public:
	static Log *instance();

	void set_status(const std::string& url, const std::string& response_code, const std::string& handler, const std::string& url_prefix);

	std::vector<status> get_statuses() const;
};

#endif /*log_h*/