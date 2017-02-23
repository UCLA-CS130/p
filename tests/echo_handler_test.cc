#include "gtest/gtest.h"
#include "../echo_handler.h"

TEST(EchoHandlerTest, TestAll) {
	std::string s("GET /team-:p HTTP/1.1\r\nHost: www.team-:p.org\r\n\r\n");
	auto req = Request::Parse(s); 
	Response res;
	std::unique_ptr<EchoHandler> handler(new EchoHandler());
	handler->HandleRequest(*req, &res);
	EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Length: 48\r\nContent-Type: text/plain\r\n\r\nGET /team-:p HTTP/1.1\r\nHost: www.team-:p.org\r\n\r\n", res.ToString());
}
