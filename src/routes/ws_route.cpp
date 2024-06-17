#include "routes.hpp"
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "../websocket/utils.hpp"
#include "../stream/stream_text.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
using tcp = boost::asio::ip::tcp;

void Routes::wsRoute(std::shared_ptr<tcp::socket> socket, std::shared_ptr<boost::beast::http::request<boost::beast::http::string_body>> req, std::shared_ptr<beast::flat_buffer> /*buffer*/ )
{

    std::string clientIp = socket->remote_endpoint().address().to_string();
    std::cout << "[CLIENT IP : " << clientIp << " ]" << std::endl;
    std::cout << "[REQUEST   : " << req->method_string() << req->target() << " ]" << std::endl;

    std::shared_ptr<WSUtils> controller;
    if (req->target() == "/stream-text")
    {
        controller = std::make_shared<StreamText>();
    }
    else
    {
        std::cerr << "NO SUCH ENDPOINT EXISTS: " << req->target() << std::endl;
    }

    if (controller)
    {
        controller->wsRequest(std::move(*socket), std::move(*req));
    }
    else
    {
        std::cerr << "NO CONTROLLER FOUND FOR TARGET: " << req->target() << std::endl;
    }
    return;
}
