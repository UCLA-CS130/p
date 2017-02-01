#include "gtest/gtest.h"
#include "webserver.h"
#include <iostream>
#include <string>

using namespace std;

TEST(NginxConfigTest, SimpleConfig0) {
  EXPECT_EQ(1,1);
}

// class WebserverTest : public ::testing::Test {
// protected:
// 	Request parse_req(istream& stream){
// 		return webserver.parse_request(stream); 
// 	}
// 	unsigned short port_num = 8080;
// 	WebServer webserver(port_num, 4);
// };

TEST(WebserverTest, SimpleConfig1){
	istringstream s("GET /team-:p HTTP/1.1\r\nHost: www.team-:p.org\r\n\r\n");
	Request req = WebServer::parse_request(s);
	EXPECT_EQ(req.method, "GET");
	EXPECT_EQ(req.path, "/team-:p");
	EXPECT_EQ(req.http_version, "1.1");
	EXPECT_EQ(req.headers["Host"], "www.team-:p.org");
}