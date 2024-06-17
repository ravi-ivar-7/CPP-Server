#ifndef SRC_SERVERS_HTTP_SERVER_HPP
#define SRC_SERVERS_HTTP_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

using tcp = boost::asio::ip::tcp;

class HttpAcceptor;

class HttpServer {
public:
    HttpServer(boost::asio::io_context &ioc, unsigned short port);

    void start(unsigned short port, unsigned int thread_pool_size);
    void run();
    void stop();

private:
    boost::asio::io_context &ioc_;
    std::unique_ptr<boost::asio::io_context::work> work_;
    std::unique_ptr<HttpAcceptor> acceptor_;
    std::vector<std::unique_ptr<std::thread>> thread_pool_;
    std::mutex mutex_;
};

class HttpAcceptor {
public:
    HttpAcceptor(boost::asio::io_context &ioc, unsigned short port);

    void start();
    void stop();

private:
    void acceptConnection();

    boost::asio::io_context &ioc_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::atomic<bool> is_stopped_{false};
};

#endif // SRC_SERVERS_HTTP_SERVER_HPP
