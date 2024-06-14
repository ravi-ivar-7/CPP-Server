#ifndef SRC_REQUESTS_POST_REQUEST_HPP
#define SRC_REQUESTS_POST_REQUEST_HPP

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

void postRequest(tcp::socket &&socket, http::request<http::string_body> &&req);

#endif // SRC_REQUESTS_POST_REQUEST_HPP