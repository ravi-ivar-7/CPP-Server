#include "../../includes/utils/HttpAcceptor.hpp"
#include "../../routers/router.hpp" // Include the header file for the router component
#include <iostream>

// Constructor: initializes the HttpAcceptor with an IO context and port number
HttpAcceptor::HttpAcceptor(boost::asio::io_context& io_context, unsigned short port)
    : m_Acceptor(io_context), m_IOContext(io_context)
{
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
    boost::asio::socket_base::reuse_address option(true);
    m_Acceptor.open(endpoint.protocol());
    m_Acceptor.set_option(option);
    m_Acceptor.bind(endpoint);
    m_Acceptor.listen();
}

// Start method: starts accepting incoming connections
void HttpAcceptor::Start()
{
    AcceptConnection(); // Start accepting connections asynchronously
}

// Stop method: stops accepting incoming connections
void HttpAcceptor::Stop()
{
    m_IsStopped.store(true); // Set the flag to true (stopped)
    m_Acceptor.close(); // Close the acceptor to stop accepting incoming connections
}

// AcceptConnection method: asynchronously accepts incoming connections
void HttpAcceptor::AcceptConnection()
{
    auto sock = std::make_shared<boost::asio::ip::tcp::socket>(m_IOContext); // Create a socket shared pointer

    // Asynchronously accept a connection
    m_Acceptor.async_accept(*sock,
        [this, sock](const boost::system::error_code& ec) // Lambda function to handle the async operation
        {
            if (!ec) // If there's no error
            {
                // Pass the accepted socket to the router component
                Router::RouteRequest(sock);
            }
            else // If there's an error
            {
                // Print an error message
                std::cerr << "ERROR: " << ec.value() << " MESSAGE: " << ec.message() << std::endl;
            }

            // If the acceptor is not stopped, continue accepting connections
            if (!m_IsStopped.load())
            {
                AcceptConnection();
            }
        });
}
