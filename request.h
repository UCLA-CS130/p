#ifndef request_h
#define request_h

#include <string>
#include <vector>

class Request {
public:
	static std::unique_ptr<Request> Parse(const std::string& raw_request);

	std::string raw_request() const;
	std::string method() const;
	std::string uri() const;
	std::string version() const;

	using Headers = std::vector<std::pair<std::string, std::string>>;
	Headers headers() const;

	std::string body() const;

private:
	Request(const std::string& raw_request) : m_raw_request(raw_request) {};

	std::string m_raw_request, m_method, m_uri, m_version;

	// std::shared_ptr<std::istream> content;

	Headers m_headers;
};

#endif /* request_h */
