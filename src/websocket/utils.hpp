#ifndef SRC_WEBSOCKET_UTILS_HPP
#define SRC_WEBSOCKET_UTILS_HPP

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = boost::asio::ip::tcp;

class WSUtils {
public:
    ~WSUtils() = default;
    void wsRequest(tcp::socket &&socket, beast::http::request<beast::http::string_body> req);

protected:
    virtual void onMessage(std::shared_ptr<websocket::stream<tcp::socket>> ws, std::string message) = 0;
    void doWrite(std::shared_ptr<websocket::stream<tcp::socket>> ws, beast::flat_buffer &buffer, std::string message);

private:
    void doAccept(std::shared_ptr<websocket::stream<tcp::socket>> ws, beast::http::request<beast::http::string_body> req);
    void doRead(std::shared_ptr<websocket::stream<tcp::socket>> ws, beast::flat_buffer &buffer);
};

#endif // SRC_WEBSOCKET_UTILS_HPP
