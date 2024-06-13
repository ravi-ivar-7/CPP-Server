#ifndef SRC_ROUTER_HPP
#define SRC_ROUTER_HPP

#include <memory>
#include <boost/asio.hpp>
#include <boost/beast.hpp>


class Router{
    public:
    static void RouteRequest(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
};

#endif //SRC_ROUTER_HPP