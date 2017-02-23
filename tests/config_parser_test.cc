#include "gtest/gtest.h"
#include "../config_parser.h"
#include <iostream>
#include <string>

TEST(NginxConfigTest, SimpleConfig0) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

class NginxConfigParserTest : public ::testing::Test {
protected:
	bool ParseFile(const char* file_name){
		return parser_.Parse(file_name, &out_config_);
	}
	bool ParseString(const std::string config_string) {
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &out_config_);
	}
	NginxConfigParser parser_;
	NginxConfig out_config_;
};

TEST_F(NginxConfigParserTest, SimpleConfig1){
	EXPECT_TRUE(ParseFile("example_config"));
}

TEST_F(NginxConfigParserTest, MissingStartBlock) {
  bool success = ParseString("server \n {listen 80;");
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MissingEndBlock) {
  bool success = ParseString("server \n listen 80; }");
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, SimpleConfig2) {
  bool success = ParseString("server \n { listen {80;} \n server_name {foo.com;} \n root /home/ubuntu/sites/foo/;}");
  EXPECT_TRUE(success);
  EXPECT_EQ(1, out_config_.statements_.size());
}

TEST_F(NginxConfigParserTest, LastNestedBlockError) {
  bool success = ParseString("server \n { listen {80;} \n server_name {foo.com;} \n root {/home/ubuntu/sites/foo/;}}");
  EXPECT_TRUE(success);
}