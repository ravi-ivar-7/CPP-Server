#include <iostream>
#include <boost/asio.hpp>
#include <cstdlib>
#include <thread>
#include "./includes/utils/HttpServer.hpp"

int main()
{
    unsigned short port = 7000;

    try
    {
        boost::asio::io_context io_context;
        HttpServer httpServer(io_context, port);

        size_t thread_pool_size = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() * 2 : 2;

        httpServer.Start(port, thread_pool_size);

        // Run the server in the main thread
        httpServer.Run();
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
    catch (...)
    {
        std::cout << "UNKNOWN ERROR" << std::endl;
    }

    return 0;
}
