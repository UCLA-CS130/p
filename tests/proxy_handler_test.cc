#include "gtest/gtest.h"
#include "../proxy_handler.h"

class ProxyHandlerTest : public ::testing::Test {
public:
    NginxConfig InitConfig() {
        std::shared_ptr<NginxConfigStatement> statement(new NginxConfigStatement);
        statement->tokens_.push_back("/");
        statement->tokens_.push_back("localhost");
        NginxConfig config;
        config.statements_.push_back(statement);
        return config;
    }
    Response GetResponse(std::string raw_request) {
        Response res;
        std::unique_ptr<Request> req = Request::Parse(raw_request);
        std::unique_ptr<ProxyHandler> handler(new ProxyHandler());
        handler->Init("/proxy", InitConfig());
        handler->HandleRequest(*req, &res);
        return res;
    }
};

TEST_F(ProxyHandlerTest,url_test){
    std::string url ="www.google.com/file/path0/test.txt";
    std::string result=ProxyHandler::request_from_url(url);
    EXPECT_EQ(result,"GET /file/path0/test.txt HTTP/1.0\r\n\r\n");
}

TEST_F(ProxyHandlerTest,filter_test){
    std::string s("GET /file/path0/test.txt HTTP/1.0\r\n\r\n");
    auto req = Request::Parse(s);
    req->m_headers={{"Cookie","foo"},{"Connection","bar"}};
    std::string result=ProxyHandler::filter_request_header(*req);

    EXPECT_EQ(result,"GET /file/path0/test.txt HTTP/1.0\r\nConnection: close\r\n\r\n");

}