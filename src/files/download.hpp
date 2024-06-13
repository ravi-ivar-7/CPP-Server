#ifndef SRC_FILES_DOWNLOAD_HPP
#define SRC_FILES_DOWNLOAD_HPP

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

void downloadFile(tcp::socket &&socket, http::request<http::string_body> &&req);

#endif // SRC_SYSTEM_SYS_SERVER_INFO_HPP