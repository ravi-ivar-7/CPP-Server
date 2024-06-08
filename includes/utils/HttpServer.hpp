#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <mutex>
#include "HttpAcceptor.hpp" // Include the HttpAcceptor header file

class HttpServer
{
public:
    HttpServer(boost::asio::io_context& io_context, unsigned short port);
    void Start(unsigned short port, unsigned int thread_pool_size);
    void Stop();
    void Run();

private:
    boost::asio::io_context& m_IOContext;
    std::unique_ptr<boost::asio::io_service::work> m_Work;
    std::unique_ptr<HttpAcceptor> m_Acceptor;
    std::vector<std::unique_ptr<std::thread>> m_ThreadPool;
    std::mutex m_Mutex;
};

#endif // HTTP_SERVER_HPP
