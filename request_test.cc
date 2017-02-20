#include "gtest/gtest.h"
#include "request.h"

TEST(RequestTest, TestAll) {
	std::string s("GET /team-:p HTTP/1.1\r\nHost: www.team-:p.org\r\n\r\n");
	auto req = Request::Parse(s);
	req->setBody("Some content.");
	EXPECT_EQ(s+"Some content.", req->raw_request());
	EXPECT_EQ("GET", req->method());
	EXPECT_EQ("/team-:p", req->uri());
	EXPECT_EQ("1.1", req->version());
	EXPECT_EQ(1, req->headers().size());
	EXPECT_EQ("Some content.", req->body());
}
