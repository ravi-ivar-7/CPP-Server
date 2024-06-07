#include "routes.hpp"
#include "../controllers/handleFiles.hpp"
#include "../controllers/common.hpp"

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

void handleSession(tcp::socket socket) {
    bool close = false;
    beast::error_code ec;

    beast::flat_buffer buffer;
    http::request<http::string_body> req;
    http::read(socket, buffer, req, ec);

    if (ec == http::error::end_of_stream) {
        close = true;
    }

    if (!ec) {
        if (req.method() == http::verb::post && req.target() == "/api/upload") {
            handleFileUpload(std::move(socket), std::move(req));
        } else if (req.method() == http::verb::get && req.target() == "/api/download") {
            handleFileDownload(std::move(socket), std::move(req));
        } else {
            handleNotFound(std::move(socket), std::move(req));
        }
    }

    if (close) {
        socket.shutdown(tcp::socket::shutdown_send, ec);
    }

    if (ec && ec != beast::errc::not_connected) {
        throw beast::system_error{ec};
    }
}
