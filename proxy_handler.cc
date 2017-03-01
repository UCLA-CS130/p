#include "proxy_handler.h"

const std::string redirect_header_ = "Location";
const std::string space_ = " ";
const std::string crlf_ = "\r\n";
const std::string name_value_separator_ = ": ";

RequestHandler::Status ProxyHandler::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{
    for (auto const& statement: config.statements_) {
        if (statement->tokens_[0] == "host") {
            if (statement->tokens_.size() == 2) {
                host_ = statement->tokens_[1];
            }
        } else if (statement->tokens_[0] == "port") {
            if (statement->tokens_.size() == 2) {
                port_ = statement->tokens_[1];
            }
        }
    }
    if (host_.length() == 0 || port_.length() == 0) return ILLEGAL_CONFIG;

    return OK;
}

RequestHandler::Status ProxyHandler::HandleRequest(const Request& request,
                               Response* response)
{
    HTTPClient client;
    try {
        client.Connect(host_, port_);

        // generate http package, http client support http 1.0 only
        std::string raw_request = filter_request_header(request);
        client.SendRequest(raw_request);
        std::cout << "send host request: " << raw_request << "\n";
        auto resp = std::move(client.GetResponse());
        std::cout << "receive host response\n";

        // handle redirect
        while (resp->GetResponseCode() == Response::MOVE_TEMPORARILY) {
            // retrieve Location which is the next url to visit
            std::string location = "";
            for(auto& header: resp->GetHeaders()) {
                if (header.first == redirect_header_) {
                    location = header.second;
                    break;
                }
            }
            if (location.length() != 0) {
                std::cout << "Redirect: " << location << "\n";

                // parse url to get host, port, path and parameter
                http::url parsed = http::ParseHttpUrl(location);
                if (parsed.port == 0) {
                    // if location does not specify port, use default 80
                    client.Connect(parsed.host);
                } else {
                    client.Connect(parsed.host, std::to_string(parsed.port));
                }

                // generate request according to Location
                raw_request = request.method() + " ";
                raw_request += parsed.path;
                if (parsed.search.length() != 0) {
                    raw_request += "?" + parsed.search;
                }
                raw_request += " HTTP/1.0\r\n\r\n";
                client.SendRequest(raw_request);

                // get response
                resp = std::move(client.GetResponse());
            } else {
                return ILLEGAL_CONFIG;
            }
        }

        // generate response
        response->SetStatus(resp->GetResponseCode());
        for (auto &header: resp->GetHeaders()) {
            response->AddHeader(header.first, header.second);
        }
        response->SetBody(resp->GetBody());
    } catch (std::exception& e) {
        std::cout << e.what();
        return ILLEGAL_CONFIG;
    }
    return OK;
}

// remove unsupported header like cookie, rewrite connection, set http version to 1.0
std::string ProxyHandler::filter_request_header(const Request& req) {
    std::stringstream ss;
    ss << req.method() << space_ << req.uri() << space_ << "HTTP/1.0" << crlf_;
    for (auto const& header: req.headers()) {
        if (header.first == "Cookie") continue; // ignore cookie
        if (header.first == "Connection") {
            ss << "Connection" << name_value_separator_ << "close" << crlf_;
            continue;
        }
        ss << header.first << name_value_separator_ << header.second << crlf_;
    }
    ss << crlf_;
    ss << req.body();
    return ss.str();
}
