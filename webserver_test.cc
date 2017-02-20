#include "gtest/gtest.h"
#include "webserver.h"
#include <iostream>
#include <string>

using namespace std;

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
