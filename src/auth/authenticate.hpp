#ifndef SRC_AUTH_AUTHENTICATE_HPP
#define SRC_AUTH_AUTHENTICATE_HPP

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

void authenticate(tcp::socket &&socket, http::request<http::string_body> &&req);

#endif // SRC_AUTH_AUTHENTICATE_HPP