#include "router.hpp"
#include "../includes/handleFiles.hpp"
#include "../includes/common.hpp"
#include "../includes/utils/sysServerInfo.hpp"
// #include "../includes/homePage.hpp"
#include <iostream>

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

void Router::RouteRequest(std::shared_ptr<tcp::socket> socket)
{
   // Read HTTP request from socket asynchronously and determine the request target
   auto req = std::make_shared<http::request<http::string_body>>();
   auto buffer = std::make_shared<beast::flat_buffer>();

   http::async_read(*socket, *buffer, *req,
                    [socket, req, buffer](beast::error_code ec, std::size_t /* bytes_transferred */)
                    {
                       if (!ec)
                       {
                          // Retrieving client IP
                          std::string clientIp = socket->remote_endpoint().address().to_string();
                          std::cout << "[CLIENT IP : " << clientIp << " ]" << std::endl;
                          std::cout << "[REQUEST   : " << req->method_string() << req->target() << " ]" << std::endl;

                          // Determine the request method and target
                          if (req->method() == http::verb::post && req->target() == "/api/upload")
                          {
                             handleFileUpload(std::move(*socket), std::move(*req));
                          }
                          else if (req->method() == http::verb::get && req->target() == "/api/download")
                          {
                             handleFileDownload(std::move(*socket), std::move(*req));
                          }
                          else if (req->method() == http::verb::get && req->target().find("/api/sysServerInfo") == 0)
                          {
                             sysServerInfo(std::move(*socket), std::move(*req));
                          }
                          else if (req->method() == http::verb::get && req->target().starts_with("/testasync"))
                          {
                             testAsync(std::move(*socket), std::move(*req));
                          }
                        //   else if (req->method() == http::verb::get && req->target() == "api/home")
                        //   {
                        //      home(std::move(*socket), std::move(*req));
                        //   }
                          else
                          {
                             handleNotFound(std::move(*socket), std::move(*req));
                          }
                       }
                       else
                       {
                          // Handle error while reading request
                          std::cerr << "Error reading request: " << ec.message() << std::endl;
                          // Close the socket to terminate the connection with the client
                          socket->close();
                       }
                    });
}
