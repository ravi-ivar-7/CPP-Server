#ifndef SRC_FILES_UPLOAD_HPP
#define SRC_FILES_UPLOAD_HPP

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

void uploadFile(tcp::socket &&socket, http::request<http::string_body> &&req);

#endif // SRC_FILES_UPLOAD_HPP
