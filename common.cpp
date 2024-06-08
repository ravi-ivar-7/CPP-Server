#include "../includes/common.hpp" 

#include <boost/beast.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace fs = boost::filesystem;
using tcp = boost::asio::ip::tcp;

void handleNotFound(tcp::socket &&socket, http::request<http::string_body> &&req)
{
    http::response<http::string_body> res{http::status::not_found, req.version()};
    res.set(http::field::server, "Beast");
    res.set(http::field::content_type, "text/plain");
    res.body() = "404 Not Found";
    res.prepare_payload();
    http::write(socket, res);
}
