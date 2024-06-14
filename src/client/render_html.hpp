#ifndef SRC_CLIENT_HOME_HPP
#define SRC_CLIENT_HOME_HPP

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

void renderHtml(tcp::socket &&socket, http::request<http::string_body> &&req);

#endif // SRC_CLIENT_HOME_HPP

