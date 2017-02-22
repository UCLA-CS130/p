#include "gtest/gtest.h"
#include "../log.h"

TEST(LogTest, TestAll) {
	Log::instance();
	Log::instance()->set_status("/foo/bar", "200", "StaticHandler", "/foo");
	Log::instance()->set_status("/foo/var", "404", "NotFoundHandler", "");
	std::vector<status> statuses = Log::instance()->get_statuses();
	EXPECT_EQ(2, statuses.size());
	EXPECT_EQ("/foo", statuses[0].s_url_prefix);
	EXPECT_EQ("404", statuses[1].s_response_code);
}