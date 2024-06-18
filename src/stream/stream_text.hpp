#ifndef STREAM_TEXT_HPP
#define STREAM_TEXT_HPP

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <mutex>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = boost::asio::ip::tcp;

#include "../websocket/utils.hpp"

class StreamText : public WSUtils
{
public:
    void onMessage(std::shared_ptr<websocket::stream<tcp::socket>> ws, const beast::http::request<beast::http::string_body> &req, std::string message) override;

protected:
    bool authenticate(const beast::http::request<beast::http::string_body> &req) override;
    std::string getUserId(const beast::http::request<beast::http::string_body> &req) override;
};

#endif // STREAM_TEXT_HPP
