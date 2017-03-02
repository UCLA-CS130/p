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

TEST_F(ProxyHandlerTest,inittest){
        std::string raw = "GET /proxy HTTP/1.0\r\nHost: localhost:8080\r\n\r\n";
        //EXPECT_EQ(req->raw_request(),"");
        EXPECT_EQ(GetResponse(raw).ToString(),"");
        //EXPECT_EQ(res,NULL);
}