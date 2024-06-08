#include <iostream>
#include <boost/asio.hpp>
#include <cstdlib>
#include <thread>
#include "./includes/utils/HttpServer.hpp"
// #include <dotenv/dotenv.h>

int main(int argc, char **argv)
{
    // dotenv::init();
    std::cout << "Number of arguments: " << argc << std::endl;

    // Print each argument
    for (int i = 0; i < argc; ++i)
    {
        std::cout << "Argument " << i << ": " << argv[i] << std::endl;
    }

    unsigned short port = 7000; // std::getenv("HTTP_PORT") ? std::stoi(std::getenv("HTTP_PORT")) : 7000;

    try
    {
        boost::asio::io_context io_context;
        HttpServer httpServer(io_context, port);

        size_t thread_pool_size = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() * 2 : 2;

        httpServer.Start(port, thread_pool_size);

        // Keep the server alive for 5 minutes
        std::this_thread::sleep_for(std::chrono::seconds(60 * 5));

        httpServer.Stop();
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
