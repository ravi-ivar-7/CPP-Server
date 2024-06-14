#ifndef SRC_REQUESTS_GET_HPP
#define SRC_REQUESTS_GET_HPP

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

void getRequest(tcp::socket &&socket, http::request<http::string_body> &&req);

#endif // SRC_REQUESTS_GET_HPP