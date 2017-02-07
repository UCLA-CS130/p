#include "gtest/gtest.h"
#include "webserver.h"
#include <iostream>
#include <string>

using namespace std;

TEST(NginxConfigTest, SimpleConfig0) {
  EXPECT_EQ(1,1);
}

class WebserverTest : public ::testing::Test {
protected:
	bool ParseFile(const char* file_name){
		return parser_.Parse(file_name, &config);
	}
	bool ParseString(const std::string config_string) {
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &config);
	}
	NginxConfigParser parser_;
	NginxConfig config;
};

TEST_F(WebserverTest, ExtractPort){
	ParseString("server \n { listen 40833; \n server_name foo.com; }");
	unsigned short port = 8080;
	extract_port(config, port);
	EXPECT_EQ(40833, port);
}

TEST_F(WebserverTest, ParseRequest){
    ParseFile("config");  
	WebServer server(config, 8080, 4);
	istringstream s("GET /team-:p HTTP/1.1\r\nHost: www.team-:p.org\r\n\r\n");
	Request req = server.parse_request(s);
	EXPECT_EQ(req.method, "GET");
	EXPECT_EQ(req.path, "/team-:p");
	EXPECT_EQ(req.http_version, "1.1");
	EXPECT_EQ(req.headers["Host"], "www.team-:p.org");
}

TEST(RequestHandlerEchoTest, GetResponse) {
	istringstream is("GET / HTTP/1.1\r\nHost: www.team-:p.org\r\n\r\n");
	Request req = WebServer::parse_request(is);
	auto paths = make_shared<unordered_set<string>>();
    paths->insert("/");
    RequestHandlerEcho handler(paths);
    boost::asio::streambuf buf;
    ostream response(&buf);
    handler.get_response(response, req);
    EXPECT_EQ(106, buf.size());
}


class RequestHandlerStaticTest : public ::testing::Test {
protected:
	void Response(istringstream& is){
		Request req = WebServer::parse_request(is);
		auto paths = make_shared<unordered_map<string, string>>();
		RequestHandlerStatic handler(paths);
		(*(handler.paths))["static"] = "file/path0";	
	    ostream response(&buf);
    	handler.get_response(response, req);
	}

	boost::asio::streambuf buf;
};

TEST_F(RequestHandlerStaticTest, GetResponse) {
	istringstream is("GET /static/image0.jpg HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
	Response(is);
    EXPECT_EQ(28025, buf.size());
}

TEST_F(RequestHandlerStaticTest, RootPath){
	istringstream is("GET /static HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
	Response(is);
	EXPECT_EQ(247, buf.size());
}

TEST_F(RequestHandlerStaticTest, HtmlFile){
	istringstream is("GET /static/test.html HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
	Response(is);
	EXPECT_EQ(228, buf.size());
}

TEST_F(RequestHandlerStaticTest, TxtFile){
	istringstream is("GET /static/test.txt HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
	Response(is);
	EXPECT_EQ(81, buf.size());
}

TEST_F(RequestHandlerStaticTest, AudioFile){
	istringstream is("GET /static/chengdu.mp3 HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
	Response(is);
	EXPECT_EQ(8668257, buf.size());
}

TEST_F(RequestHandlerStaticTest, PdfFile){
	istringstream is("GET /static/hw2.pdf HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
	Response(is);
	EXPECT_EQ(94460, buf.size());
}

TEST_F(RequestHandlerStaticTest, 404NotFound){
	istringstream is("GET /static/superbowl.mov HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
	Response(is);
	EXPECT_EQ(90, buf.size());
}

TEST_F(RequestHandlerStaticTest, NoStaticMapping){
	istringstream is("GET /static5/superbowl.mov HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
	Response(is);
	EXPECT_EQ(87, buf.size());
}

TEST_F(RequestHandlerStaticTest, FindLastDot){
	istringstream is("GET /static/superbowl.2017.mov HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
	Response(is);
	EXPECT_EQ(94, buf.size());
}