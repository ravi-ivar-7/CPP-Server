#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "http_server.hpp"
#include "../routes/routes.hpp"
#include <boost/algorithm/string.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

HttpServer::HttpServer(boost::asio::io_context &ioc, unsigned short port)
    : ioc_(ioc), work_(std::make_unique<boost::asio::io_context::work>(ioc))
{
    acceptor_ = std::make_unique<HttpAcceptor>(ioc, port);
}

void HttpServer::start(unsigned short port, unsigned int thread_pool_size)
{
    std::lock_guard<std::mutex> lock(mutex_);
    try
    {
        acceptor_->start();

        std::cout << "SERVER STARTED" << std::endl;
        // std::cout << "HTTP HOST: " << "localhost" << std::endl;
        std::cout << "HTTP PORT: " << port << std::endl;

        for (size_t i = 0; i < thread_pool_size; ++i)
        {
            std::unique_ptr<std::thread> new_thread(new std::thread([this]()
                                                                    { ioc_.run(); }));
            thread_pool_.push_back(std::move(new_thread));
        }
    }

    catch (const boost::system::system_error &e)
    {
        std::cerr << "BOOST SYSTEM ERROR: " << e.code() << " -- " << e.what() << std::endl;
    }
    catch (const std::logic_error &e)
    {
        std::cerr << "LOGIC ERROR: " << e.what() << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "RUNTIME ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "UNKNOWN ERROR" << std::endl;
    }
}

void HttpServer::stop()
{
    std::lock_guard<std::mutex> lock(mutex_);
    try
    {
        if (acceptor_)
            acceptor_->stop();

        ioc_.stop();

        std::cout << "SERVER STOPPED!" << std::endl;

        for (auto &thread : thread_pool_)
        {
            if (thread && thread->joinable())
                thread->join();
        }
    }

    catch (const boost::system::system_error &e)
    {
        std::cerr << "BOOST SYSTEM ERROR: " << e.code() << " -- " << e.what() << std::endl;
    }
    catch (const std::logic_error &e)
    {
        std::cerr << "LOGIC ERROR: " << e.what() << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "RUNTIME ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "UNKNOWN ERROR" << std::endl;
    }
}

// Run the server (blocking call)
void HttpServer::run()
{
    ioc_.run();
}

HttpAcceptor::HttpAcceptor(boost::asio::io_context &ioc, unsigned short port)
    : ioc_(ioc), acceptor_(ioc)
{
    // boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);

boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

    boost::asio::socket_base::reuse_address option(true); // Set the socket option to reuse the address

    acceptor_.open(endpoint.protocol());

    acceptor_.set_option(option);

    acceptor_.bind(endpoint);

    acceptor_.listen();
}

void HttpAcceptor::start()
{
    acceptConnection();
}

void HttpAcceptor::stop()
{
    is_stopped_.store(true);
    acceptor_.close();
}

void HttpAcceptor::acceptConnection()
{
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(ioc_); // new socket for each incoming connection

    acceptor_.async_accept(*socket, [this, socket](const boost::system::error_code &ec)
                           {
        if (!ec) {
            auto buffer = std::make_shared<beast::flat_buffer>();
            auto req = std::make_shared<http::request<http::string_body>> ();

            http::async_read(*socket, *buffer, *req, [this, socket, buffer, req](beast::error_code ec, std::size_t /*bytes_transferred*/){
                if(!ec){
                    if(websocket::is_upgrade(*req))Routes::wsRoute(socket, req, buffer);
                    else Routes::httpRoute(socket, req, buffer);
                }
                else{
                    std::cerr << "ERROR: " << ec.message() << std::endl;
                }
            });

        } else {
            std::cerr << "ERROR: " << ec.value() << " MESSAGE: " << ec.message() << std::endl;
        }

        if (!is_stopped_.load()) {
            acceptConnection(); // continue accepting connections
        } });
}