#ifndef RENDER_FILE_HPP
#define RENDER_FILE_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

void renderFile(tcp::socket &&socket, http::request<http::string_body> &&req);

#endif
