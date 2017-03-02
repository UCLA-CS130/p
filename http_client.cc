#include "http_client.h"

HTTPClient::HTTPClient(): io_service_(),
socket_(io_service_)
{

}

bool HTTPClient::Connect(std::string host, std::string port) {
    try {
        boost::asio::ip::tcp::resolver resolver(io_service_);
        boost::asio::ip::tcp::resolver::query query(host, port);
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        boost::asio::connect(socket_, endpoint_iterator);
    }catch (std::exception& e) {
        printf("will write\n");
        std::cout << e.what() << "\n";
        return false;
    }
    return true;
}

bool HTTPClient::SendRequest(std::string request) {
    boost::asio::streambuf requestbuf;
    std::ostream request_stream(&requestbuf);
    request_stream << request;
    return boost::asio::write(socket_, requestbuf) == request.length();
}

std::unique_ptr<Response> HTTPClient::GetResponse() {
    boost::asio::streambuf resp;
    boost::system::error_code error;
    std::unique_ptr<Response> response;
    try {
        boost::asio::read_until(socket_, resp, "\r\n\r\n");
        std::string str( (std::istreambuf_iterator<char>(&resp)), std::istreambuf_iterator<char>() );
        response = Response::ParseHeader(str);
        while (boost::asio::read(socket_, resp, boost::asio::transfer_at_least(1), error)) {
            if (error) break;
        }
        std::string body( (std::istreambuf_iterator<char>(&resp)), std::istreambuf_iterator<char>() );
        std::string whole = str+body;
        body = whole.substr(whole.find("\r\n\r\n")+4);
        response->SetBody(body);
    } catch(std::exception& e) {
        std::cout << e.what();
        throw e;
    }

    return response;
}

HTTPClient::~HTTPClient() {
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
}
