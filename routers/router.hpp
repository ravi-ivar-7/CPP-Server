#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <memory> // For std::shared_ptr
#include <boost/asio.hpp> // For Boost.Asio components
#include <boost/beast.hpp> // For Boost.Beast components

class Router {
public:
    // RouteRequest function to handle incoming HTTP requests
    static void RouteRequest(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
};

#endif // ROUTER_HPP
