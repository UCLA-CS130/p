#include "gtest/gtest.h"
#include "webserver.h"
#include <iostream>
#include <string>

using namespace std;

TEST(NginxConfigTest, SimpleConfig0) {
  EXPECT_EQ(1,1);
}

TEST(WebserverTest, ParseRequest){
	istringstream s("GET /team-:p HTTP/1.1\r\nHost: www.team-:p.org\r\n\r\n");
	WebServer server(8080, 4);
	Request req = server.parse_request(s);
	EXPECT_EQ(req.method, "GET");
	EXPECT_EQ(req.path, "/team-:p");
	EXPECT_EQ(req.http_version, "1.1");
	EXPECT_EQ(req.headers["Host"], "www.team-:p.org");
}