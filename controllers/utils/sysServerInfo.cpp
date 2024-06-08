#include "../../includes/utils/sysServerInfo.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

namespace http = boost::beast::http;

// Function to get CPU information
std::string getCPUInfo() {
    try {
        std::ifstream cpuInfoFile("/proc/cpuinfo");
        if (!cpuInfoFile.is_open()) {
            throw std::runtime_error("Failed to open CPU info file");
        }
        
        std::stringstream buffer;
        buffer << cpuInfoFile.rdbuf();
        return buffer.str();
    } catch (const std::exception& e) {
        std::cerr << "Error while getting CPU info: " << e.what() << std::endl;
        return "Error getting CPU info";
    }
}

// Function to get memory information
std::string getMemoryInfo() {
    try {
        std::ifstream memInfoFile("/proc/meminfo");
        if (!memInfoFile.is_open()) {
            throw std::runtime_error("Failed to open memory info file");
        }
        
        std::stringstream buffer;
        buffer << memInfoFile.rdbuf();
        return buffer.str();
    } catch (const std::exception& e) {
        std::cerr << "Error while getting memory info: " << e.what() << std::endl;
        return "Error getting memory info";
    }
}

// Function to get system uptime
long getUptime() {
    try {
        std::ifstream uptimeFile("/proc/uptime");
        if (!uptimeFile.is_open()) {
            throw std::runtime_error("Failed to open uptime file");
        }
        
        long uptime;
        uptimeFile >> uptime;
        return uptime;
    } catch (const std::exception& e) {
        std::cerr << "Error while getting uptime: " << e.what() << std::endl;
        return -1; // Return a default value or handle the error as appropriate
    }
}

// Function to get the number of CPU cores
int getNumCores() {
    try {
        int cores = sysconf(_SC_NPROCESSORS_ONLN);
        if (cores == -1) {
            throw std::runtime_error("Failed to get number of CPU cores");
        }
        return cores;
    } catch (const std::exception& e) {
        std::cerr << "Error while getting number of CPU cores: " << e.what() << std::endl;
        return -1; // Return a default value or handle the error as appropriate
    }
}

void sysServerInfo(tcp::socket &&socket, http::request<http::string_body> &&req) {
    try {
        // Extract the query string
        std::string target = req.target().to_string();
        std::string query_string;
        size_t pos = target.find('?');
        if (pos != std::string::npos) {
            query_string = target.substr(pos + 1);
        }

        // Parse the query parameters
        bool sysinfo = false;
        if (query_string.find("sysinfo") != std::string::npos) {
            sysinfo = true;
        }

        std::string response;

        if (sysinfo) {
            std::string cpuInfo = getCPUInfo();
            std::string memInfo = getMemoryInfo();
            long uptime = getUptime();
            int cores = getNumCores();

            response = R"({"status": "ok", "message": "Server is healthy", "cpu_info": ")" + cpuInfo + R"(", "mem_info": ")" + memInfo + R"(", "uptime": )" + std::to_string(uptime) + R"(, "cores": )" + std::to_string(cores) + "}";
        } else {
            response = R"({"status": "ok", "message": "Server is healthy"})";
        }

        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = response;
        res.prepare_payload();
        http::write(socket, res);
    } catch (const std::exception& e) {
        std::cerr << "Error while processing request: " << e.what() << std::endl;
        // Respond with an error message
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::content_type, "text/plain");
        res.body() = "Internal Server Error: " + std::string(e.what());
        res.prepare_payload();
        http::write(socket, res);
    }
}


void testAsync(tcp::socket&& socket, http::request<http::string_body>&& req)
{
    // Simulate a delay of 10-20 seconds using a background thread
    std::thread([socket = std::move(socket), req = std::move(req)]() mutable {
        std::cout<<"request reached"<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(15)); // Delay for 15 seconds
        std::cout<<"request completed" <<std::endl;
        // Create a success response
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"status": "success", "message": "Delayed response"})";
        res.prepare_payload();

        // Write the response back to the client
        http::write(socket, res);
    }).detach();
}