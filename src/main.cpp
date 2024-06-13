#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "./servers/http_server.hpp"
// #include "./utils/catch_helper.hpp"

int main()
{
    unsigned short port = 7000;

    try
    {
        boost::asio::io_context ioc;
        HttpServer httpServer(ioc, port);

        size_t thread_pool_size = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() * 2 : 2;

        httpServer.start(port, thread_pool_size);

        httpServer.run(); // this main thread is dedicated to run the io_context event loop(this thread is blocked, while other will keeps accepting new connetions)
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

    return 0;
}
