#include "gtest/gtest.h"
#include "../status_handler.h"

TEST(StatusHandlerTest, TestAll) {
	Log::instance();
	Log::instance()->set_status("/foo/bar", "200", "StaticHandler", "/foo");
	Log::instance()->set_status("/foo/var", "404", "NotFoundHandler", "");
	std::string s("GET /status/ HTTP/1.1\r\nHost: www.team-:p.org\r\n\r\n");
	auto req = Request::Parse(s); 
	Response res;
	std::unique_ptr<StatusHandler> handler(new StatusHandler());
	handler->HandleRequest(*req, &res);
	std::string expected = "HTTP/1.1 200 OK\r\nContent-Length: 227\r\nContent-Type: text/plain\r\n\r\n2 requests the server has served.\r\n\r\nRequested URL: /foo/bar\r\nResponse code: 200\r\nRequest handler: StaticHandler\r\nFor prefix: /foo\r\n\r\nRequested URL: /foo/var\r\nResponse code: 404\r\nRequest handler: NotFoundHandler\r\nFor prefix: \r\n";
	EXPECT_EQ(expected, res.ToString());
}