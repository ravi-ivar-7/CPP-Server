#ifndef SRC_SERVERS_HTTP_SERVER_HPP
#define SRC_SERVERS_HTTP_SERVER_HPP

#include <boost/asio.hpp>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class HttpAcceptor;


class HttpServer
{
public:
    HttpServer(boost::asio::io_context &ioc, unsigned short port);

    void start(unsigned short port, unsigned int thread_pool_size);
    void run();
    void stop();

private:
    boost::asio::io_context &ioc_;                          // reference to the shared io_context
    std::unique_ptr<boost::asio::io_context::work> work_;   // ensures io_context runs as long as work exists
    std::unique_ptr<HttpAcceptor> acceptor_;                // pointer to HttpAcceptor for accepting connections
    std::vector<std::unique_ptr<std::thread>> thread_pool_; // thread pool for handling async operations
    std::mutex mutex_;                                      // mutex for thread safety
};

class HttpAcceptor
{
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