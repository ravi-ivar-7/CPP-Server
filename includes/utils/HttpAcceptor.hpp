#ifndef HTTP_ACCEPTOR_HPP
#define HTTP_ACCEPTOR_HPP

#include <boost/asio.hpp>
#include <atomic>
#include <memory>

class HttpAcceptor {
public:
    HttpAcceptor(boost::asio::io_context& io_context, unsigned short port);

    void Start();
    void Stop();

private:
    void AcceptConnection();

    boost::asio::ip::tcp::acceptor m_Acceptor;
    boost::asio::io_context& m_IOContext;
    std::atomic<bool> m_IsStopped{false};
};

#endif // HTTP_ACCEPTOR_HPP
