#ifndef HOME_PAGE_HPP
#define HOME_PAGE_HPP

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

void homePage(tcp::socket &&socket, http::request<http::string_body> &&req);

#endif // HOME_CONTROLLER_HPP
