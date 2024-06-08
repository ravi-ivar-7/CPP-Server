#include "../../includes/utils/HttpServer.hpp"
#include "../../includes/utils/HttpAcceptor.hpp"
#include <iostream>
#include <stdexcept>

HttpServer::HttpServer(boost::asio::io_context& io_context, unsigned short port)
    : m_IOContext(io_context),
      m_Work(std::make_unique<boost::asio::io_service::work>(io_context)) // Create work to prevent io_service from stopping prematurely
{
    // Create and initialize the HttpAcceptor
    m_Acceptor = std::make_unique<HttpAcceptor>(io_context, port);
}

void HttpServer::Start(unsigned short port, unsigned int thread_pool_size)
{
    if (thread_pool_size <= 0)
        return;

    std::lock_guard<std::mutex> lock(m_Mutex); // Ensure thread safety

    try
    {
        // Start the HttpAcceptor for accepting connection requests
        m_Acceptor->Start();

        // Server information
        std::cout << "SERVER STARTED" << std::endl;
        std::cout << "HTTP HOST: " << "localhost" << std::endl;
        std::cout << "HTTP PORT: " << port << std::endl;

        // Create specified number of threads and add them to the pool
        for (size_t i = 0; i < thread_pool_size; ++i)
        {
            std::unique_ptr<std::thread> new_thread(new std::thread([this]()
                                                                    { m_IOContext.run(); }));
            m_ThreadPool.push_back(std::move(new_thread));
        }
    }
    catch (boost::system::system_error &e)
    {
        std::cout << "ERROR CODE: " << e.code() << " ERROR MESSAGE: " << e.what() << std::endl;
    }
    catch (std::logic_error &e)
    {
        std::cout << "LOGIC ERROR: " << e.what() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
    }
}

void HttpServer::Stop()
{
    std::lock_guard<std::mutex> lock(m_Mutex); // Ensure thread safety

    try
    {
        // Stop accepting new connections
        if (m_Acceptor)
        {
            m_Acceptor->Stop();
        }

        // Stop the io_service event loop
        m_IOContext.stop();

        std::cout << "SERVER STOPPED!" << std::endl;

        // Join all threads in the thread pool
        for (auto &thread : m_ThreadPool)
        {
            if (thread && thread->joinable())
                thread->join();
        }
    }
    catch (boost::system::system_error &e)
    {
        std::cout << "ERROR CODE: " << e.code() << " ERROR MESSAGE: " << e.what() << std::endl;
    }
    catch (std::logic_error &e)
    {
        std::cout << "LOGIC ERROR: " << e.what() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
    }
}

void HttpServer::Run()
{
    m_IOContext.run();
}
