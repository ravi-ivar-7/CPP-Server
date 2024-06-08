#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

void handleFileUpload(tcp::socket &&socket, http::request<http::string_body> &&req);
void handleFileDownload(tcp::socket &&socket, http::request<http::string_body> &&req);

#endif // CONTROLLER_HPP
