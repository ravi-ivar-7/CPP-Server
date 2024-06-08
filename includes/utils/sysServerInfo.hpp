#ifndef SYS_SERVER_INFO_HPP
#define SYS_SERVER_INFO_HPP

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <thread>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

void sysServerInfo(tcp::socket &&socket, http::request<http::string_body> &&req);

void testAsync(tcp::socket &&socket, http::request<http::string_body> &&req);

#endif // SYS_SERVER_INFO_HPP
