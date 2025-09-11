#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <chrono>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
using json = nlohmann::json;

class HealthCheck {
private:
    static std::chrono::steady_clock::time_point start_time_;
    
public:
    static void init() {
        start_time_ = std::chrono::steady_clock::now();
    }
    
    static void handleHealthCheck(
        std::shared_ptr<boost::asio::ip::tcp::socket> socket,
        std::shared_ptr<http::request<http::string_body>> req,
        std::shared_ptr<beast::flat_buffer> buffer
    );
    
    static json getHealthStatus();
    static std::string getUptime();
};

// Static member declaration only - definition in .cpp file
