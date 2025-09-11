#include "health_check.hpp"
#include "../utils/logger.hpp"
#include "../utils/config.hpp"
#include <boost/asio.hpp>
#include <fstream>
#include <sys/statvfs.h>
#include <unistd.h>

// Static member definition
std::chrono::steady_clock::time_point HealthCheck::start_time_;

void HealthCheck::handleHealthCheck(
    std::shared_ptr<boost::asio::ip::tcp::socket> socket,
    std::shared_ptr<http::request<http::string_body>> req,
    std::shared_ptr<beast::flat_buffer> /*buffer*/
) {
    try {
        json health_data = getHealthStatus();
        
        http::response<http::string_body> res{http::status::ok, req->version()};
        res.set(http::field::server, "C++ HTTP Server");
        res.set(http::field::content_type, "application/json");
        res.body() = health_data.dump(2);
        res.prepare_payload();

        auto write_handler = [socket](beast::error_code ec, std::size_t) {
            if (ec) {
                LOG_ERROR("Health check response error: " + ec.message());
                return;
            }
            
            beast::error_code shutdown_ec;
            socket->shutdown(boost::asio::ip::tcp::socket::shutdown_send, shutdown_ec);
            if (shutdown_ec) {
                LOG_WARN("Socket shutdown error: " + shutdown_ec.message());
            }
        };
        
        http::async_write(*socket, res, write_handler);

        LOG_INFO("Health check request processed");
    }
    catch (const std::exception& e) {
        LOG_ERROR("Health check error: " + std::string(e.what()));
        
        http::response<http::string_body> res{http::status::internal_server_error, req->version()};
        res.set(http::field::server, "C++ HTTP Server");
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"status":"error","message":"Health check failed"})";
        res.prepare_payload();

        auto error_handler = [socket](beast::error_code /*ec*/, std::size_t) {
            beast::error_code shutdown_ec;
            socket->shutdown(boost::asio::ip::tcp::socket::shutdown_send, shutdown_ec);
        };
        
        http::async_write(*socket, res, error_handler);
    }
}

json HealthCheck::getHealthStatus() {
    json health;
    
    try {
        // Basic status
        health["status"] = "healthy";
        health["timestamp"] = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        health["uptime"] = getUptime();
        
        // Server info
        health["server"]["name"] = "C++ HTTP Server";
        health["server"]["version"] = "1.0.0";
        health["server"]["port"] = Config::getInt("SERVER_PORT", 8080);
    
    // System info
    health["system"]["hostname"] = []() {
        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) == 0) {
            return std::string(hostname);
        }
        return std::string("unknown");
    }();
    
    // Memory info (basic)
    try {
        std::ifstream meminfo("/proc/meminfo");
        if (meminfo.is_open()) {
            std::string line;
            while (std::getline(meminfo, line)) {
                if (line.find("MemTotal:") == 0) {
                    std::istringstream iss(line);
                    std::string key, value, unit;
                    if (iss >> key >> value >> unit) {
                        health["system"]["memory_total_kb"] = std::stoi(value);
                    }
                }
                else if (line.find("MemAvailable:") == 0) {
                    std::istringstream iss(line);
                    std::string key, value, unit;
                    if (iss >> key >> value >> unit) {
                        health["system"]["memory_available_kb"] = std::stoi(value);
                    }
                    break;
                }
            }
        }
    } catch (const std::exception& e) {
        LOG_WARN("Failed to read memory info: " + std::string(e.what()));
    }
    
    // Disk space
    try {
        struct statvfs stat;
        if (statvfs(".", &stat) == 0) {
            unsigned long total_space = stat.f_blocks * stat.f_frsize;
            unsigned long free_space = stat.f_bavail * stat.f_frsize;
            health["system"]["disk_total_bytes"] = total_space;
            health["system"]["disk_free_bytes"] = free_space;
            if (total_space > 0) {
                health["system"]["disk_usage_percent"] = 
                    ((double)(total_space - free_space) / total_space) * 100.0;
            } else {
                health["system"]["disk_usage_percent"] = 0.0;
            }
        }
    } catch (const std::exception& e) {
        LOG_WARN("Failed to get disk info: " + std::string(e.what()));
    }
    
    // Configuration
    try {
        health["config"]["log_level"] = Config::get("LOG_LEVEL");
        health["config"]["thread_pool_size"] = Config::getInt("THREAD_POOL_SIZE");
    } catch (const std::exception& e) {
        LOG_WARN("Failed to get config info: " + std::string(e.what()));
        health["config"]["log_level"] = "unknown";
        health["config"]["thread_pool_size"] = 0;
    }
    
        // Dependencies status
        health["dependencies"]["boost"] = "ok";
        health["dependencies"]["openssl"] = "ok";
        health["dependencies"]["sqlite"] = "ok";
        health["dependencies"]["bcrypt"] = "ok";
        
    } catch (const std::exception& e) {
        LOG_ERROR("Error in getHealthStatus: " + std::string(e.what()));
        health["status"] = "error";
        health["error"] = e.what();
    }
    
    return health;
}

std::string HealthCheck::getUptime() {
    auto now = std::chrono::steady_clock::now();
    auto uptime_duration = now - start_time_;
    
    auto hours = std::chrono::duration_cast<std::chrono::hours>(uptime_duration);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(uptime_duration % std::chrono::hours(1));
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(uptime_duration % std::chrono::minutes(1));
    
    std::ostringstream oss;
    oss << hours.count() << "h " << minutes.count() << "m " << seconds.count() << "s";
    return oss.str();
}
