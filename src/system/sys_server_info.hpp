#ifndef SRC_SYSTEM_SYS_SERVER_INFO_HPP
#define SRC_SYSTEM_SYS_SERVER_INFO_HPP

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

void sysServerInfo(tcp::socket &&socket, http::request<http::string_body> &&req);

#endif // SRC_SYSTEM_SYS_SERVER_INFO_HPP
