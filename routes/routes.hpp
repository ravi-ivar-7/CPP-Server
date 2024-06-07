#ifndef ROUTES_HPP
#define ROUTES_HPP

#include <boost/asio/ip/tcp.hpp>

using tcp = boost::asio::ip::tcp;

void handleSession(tcp::socket socket);

#endif // ROUTES_HPP
