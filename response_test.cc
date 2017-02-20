#include "gtest/gtest.h"
#include "response.h"

TEST(ResponseTest, TestAll) {
	Response res;
	res.SetStatus(Response::ResponseCode::OK);
	res.AddHeader("Content-length", "16");
	res.SetBody("This is content.");
	EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-length: 16\r\n\r\nThis is content.", res.ToString());
}