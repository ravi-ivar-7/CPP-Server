#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "./servers/http_server.hpp"
#include "./utils/config.hpp"
#include "./utils/logger.hpp"
#include "./health/health_check.hpp"

int main()
{
    // Initialize configuration
    Config::init("config/server.conf");
    
    // Initialize logger
    Logger::init(Config::get("LOG_FILE"), 
                Config::get("LOG_LEVEL") == "DEBUG" ? LogLevel::DEBUG :
                Config::get("LOG_LEVEL") == "WARN" ? LogLevel::WARN :
                Config::get("LOG_LEVEL") == "ERROR" ? LogLevel::ERROR : LogLevel::INFO,
                true, !Config::get("LOG_FILE").empty());
    
    // Initialize health check
    HealthCheck::init();
    
    unsigned short port = Config::getInt("SERVER_PORT", 8080);

    try
    {
        boost::asio::io_context ioc;
        HttpServer httpServer(ioc, port);

        size_t thread_pool_size = Config::getInt("THREAD_POOL_SIZE", 
            std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() * 2 : 2);

        LOG_INFO("Starting C++ HTTP Server on port " + std::to_string(port));
        LOG_INFO("Thread pool size: " + std::to_string(thread_pool_size));
        
        httpServer.start(port, thread_pool_size);

        httpServer.run(); // this main thread is dedicated to run the io_context event loop(this thread is blocked, while other will keeps accepting new connetions)
    }
    
    catch (const boost::system::system_error &e)
    {
        LOG_FATAL("Boost system error: " + std::string(e.what()) + " (code: " + std::to_string(e.code().value()) + ")");
    }
    catch (const std::logic_error &e)
    {
        LOG_FATAL("Logic error: " + std::string(e.what()));
    }
    catch (const std::runtime_error &e)
    {
        LOG_FATAL("Runtime error: " + std::string(e.what()));
    }
    catch (const std::exception &e)
    {
        LOG_FATAL("Exception: " + std::string(e.what()));
    }
    catch (...)
    {
        LOG_FATAL("Unknown error occurred");
    }

    Logger::cleanup();
    return 0;
}
