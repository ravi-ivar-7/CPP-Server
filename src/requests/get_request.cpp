#include <boost/asio.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <unordered_map>

#include "utils.hpp"
#include "get_request.hpp"
#include "../system/log.hpp"

namespace asio = boost::asio;
namespace ssl = asio::ssl;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = asio::ip::tcp;

void getRequest(tcp::socket &&socket, http::request<http::string_body> &&req)
{
    try
    {
        std::string target = req.target().to_string();
        size_t pos = target.find('?');
        std::string host;
        std::string endpoint;
        std::string port = "443";

        if (pos != std::string::npos)
        {
            std::unordered_map<std::string, std::string> queryParams;
            std::string queryString = target.substr(pos + 1);
            queryParams = getQueryParams(queryString);
            auto host_it = queryParams.find("host");
            auto ep_it = queryParams.find("ep");
            if (host_it != queryParams.end() && ep_it != queryParams.end())
            {
                host = host_it->second;
                endpoint = ep_it->second;
            }
        }

        if (host.empty() || endpoint.empty()) {
            throw std::invalid_argument("Missing host or endpoint in query parameters");
        }

        // The io_context is required for all I/O
        asio::io_context ioc;

        // The SSL context is required, and holds certificates
        ssl::context ctx(ssl::context::tlsv12_client);

        // Set SSL context options
        ctx.set_options(
            ssl::context::default_workarounds |
            ssl::context::no_sslv2 |
            ssl::context::no_sslv3 |
            ssl::context::single_dh_use);

        // These objects perform our I/O
        tcp::resolver resolver(ioc);
        ssl::stream<tcp::socket> stream(ioc, ctx);

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        asio::connect(stream.next_layer(), results.begin(), results.end());

        // Set SNI Hostname (many hosts need this to handshake successfully)
        if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str()))
        {
            beast::error_code ec{static_cast<int>(::ERR_get_error()), asio::error::get_ssl_category()};
            throw beast::system_error{ec};
        }

        // Perform the SSL handshake
        stream.handshake(ssl::stream_base::client);

        // Set up an HTTP GET request message
        http::request<http::string_body> http_req{http::verb::get, endpoint, 11};
        http_req.set(http::field::host, host);
        http_req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Send the HTTP request to the remote host
        http::write(stream, http_req);

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::dynamic_body> response;

        // Receive the HTTP response
        http::read(stream, buffer, response);

        auto const response_body = beast::buffers_to_string(response.body().data());

        // Construct response to send back to the client
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = response_body;
        res.prepare_payload();
        http::write(socket, res);

        // Gracefully close the socket
        beast::error_code ec;
        socket.shutdown(tcp::socket::shutdown_send, ec);
    }
    catch (const std::exception &ex)
    {
        saveLog("ERROR", ex.what());
        std::cerr << "Internal Server Error: " << ex.what() << std::endl;
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::content_type, "text/plain");
        res.body() = "Internal Server Error: " + std::string(ex.what());
        res.prepare_payload();
        http::write(socket, res);
    }
}
