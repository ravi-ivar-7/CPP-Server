#ifndef SRC_WEBSOCKET_UTILS_HPP
#define SRC_WEBSOCKET_UTILS_HPP

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = boost::asio::ip::tcp;

class WSUtils
{
public:
    ~WSUtils() = default;
    void wsRequest(tcp::socket &&socket, beast::http::request<beast::http::string_body> req);

protected:
    virtual void onMessage(std::shared_ptr<websocket::stream<tcp::socket>> ws, const beast::http::request<beast::http::string_body> &req, std::string message) = 0;
    virtual void doWrite(std::shared_ptr<websocket::stream<tcp::socket>> ws,std::string message);
    virtual bool authenticate(const beast::http::request<beast::http::string_body> &req);
    virtual std::string getUserId(const beast::http::request<beast::http::string_body> &req);

    static std::unordered_map<std::string, std::shared_ptr<websocket::stream<tcp::socket>>> activeSessions;
    static std::mutex sessionsMutex;

private:
    void doAccept(std::shared_ptr<websocket::stream<tcp::socket>> ws, beast::http::request<beast::http::string_body> req);
    void doRead(std::shared_ptr<websocket::stream<tcp::socket>> ws, beast::http::request<beast::http::string_body> req, beast::flat_buffer &buffer);
};

#endif // SRC_WEBSOCKET_UTILS_HPP
