#include "gtest/gtest.h"
#include "webserver.h"
#include <iostream>
#include <string>

using namespace std;

TEST(NginxConfigTest, SimpleConfig0) {
  EXPECT_EQ(1,1);
}

TEST(WebserverTest, ParseRequest){
	// auto p1 = make_shared<RequestHandlerEcho>(nullptr);
	// auto p2 = make_shared<RequestHandlerStatic>(nullptr); 
	WebServer server(8080, nullptr, nullptr, 4);
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
