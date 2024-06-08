#ifndef COMMON_HPP
#define COMMON_HPP

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

void handleNotFound(tcp::socket &&socket, http::request<http::string_body> &&req);

#endif // COMMON_HPP
