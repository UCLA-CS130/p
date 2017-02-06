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

TEST(RequestHandlerEcho, GetResponse) {
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


// TEST(RequestHandlerStatic, GetResponse) {
	
// }
