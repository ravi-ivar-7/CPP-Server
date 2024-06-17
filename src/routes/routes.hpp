#ifndef ROUTES_HPP
#define ROUTES_HPP

#include <memory>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class Routes {
public:
    static void wsRoute(std::shared_ptr<tcp::socket> socket, std::shared_ptr<boost::beast::http::request<boost::beast::http::string_body>> req, std::shared_ptr<beast::flat_buffer> buffer);
    static void httpRoute(std::shared_ptr<tcp::socket> socket, std::shared_ptr<boost::beast::http::request<boost::beast::http::string_body>> req, std::shared_ptr<beast::flat_buffer> buffer);

};

#endif // ROUTES_HPP
