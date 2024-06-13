#ifndef SRC_UTILS_NOT_FOUND_HPP
#define SRC_UTILS_NOT_FOUND_HPP

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

void notFound(tcp::socket &&socket, http::request<http::string_body> &&req)
{
    http::response<http::string_body> res{http::status::not_found, req.version()};
    res.set(http::field::server, "Beast");
    res.set(http::field::content_type, "text/plain");
    res.body() = "404 Not Found";
    res.prepare_payload();
    http::write(socket, res);
}

#endif // SRC_UTILS_NOT_FOUND_HPP
