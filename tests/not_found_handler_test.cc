#include "gtest/gtest.h"
#include "../not_found_handler.h"

TEST(NotFoundHandlerTest, TestAll) {
	auto req = Request::Parse("GET /team"); 
	Response res;
	std::unique_ptr<NotFoundHandler> handler(new NotFoundHandler());
	handler->HandleRequest(*req, &res);
	EXPECT_EQ("HTTP/1.1 404 NOT FOUND\r\nContent-Length: 14\r\nContent-Type: text/plain\r\n\r\n404 Not Found.", res.ToString());
}