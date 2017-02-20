#include "gtest/gtest.h"
#include "../static_handler.h"

class StaticHandlerTest : public ::testing::Test {
public:
	NginxConfig InitConfig() {
		std::shared_ptr<NginxConfigStatement> statement(new NginxConfigStatement);
		statement->tokens_.push_back("root");
		statement->tokens_.push_back("file/path0");
		NginxConfig config;
		config.statements_.push_back(statement);
		return config;
	}

	Response GetResponse(const std::string &file) {
		std::string s = "GET /static/"+file+" HTTP/1.0\r\nHost: localhost:8080\r\n\r\n";
		auto req = Request::Parse(s);
		Response res;
		std::unique_ptr<StaticHandler> handler(new StaticHandler());
		handler->Init("/static", InitConfig());
		handler->HandleRequest(*req, &res);
		return res;
	}
};

TEST_F (StaticHandlerTest, IllegalConfig) {
	std::shared_ptr<NginxConfigStatement> statement(new NginxConfigStatement);
	statement->tokens_.push_back("root");
	NginxConfig config;
	config.statements_.push_back(statement);
	std::unique_ptr<StaticHandler> handler(new StaticHandler());
	EXPECT_EQ(RequestHandler::Status::ILLEGAL_CONFIG, handler->Init("/static", config));
}

TEST_F(StaticHandlerTest, ImageFile) {
	Response res = GetResponse("image0.jpg");
	EXPECT_EQ(28025, res.ToString().size());
}

TEST_F(StaticHandlerTest, HtmlFile) {
	Response res = GetResponse("test.html");
	EXPECT_EQ(228, res.ToString().size());
}

TEST_F(StaticHandlerTest, TxtFile) {
	Response res = GetResponse("test.txt");
	EXPECT_EQ(81, res.ToString().size());
}

TEST_F(StaticHandlerTest, AudioFile) {
	Response res = GetResponse("chengdu.mp3");
	EXPECT_EQ(8668257, res.ToString().size());
}

TEST_F(StaticHandlerTest, PdfFile) {
	Response res = GetResponse("hw2.pdf");
	EXPECT_EQ(94460, res.ToString().size());
}

TEST_F(StaticHandlerTest, 404NotFound) {
	Response res = GetResponse("superbowl.mov");
	EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Length: 14\r\nContent-Type: text/plain\r\n\r\n404 Not Found.", res.ToString());
}
