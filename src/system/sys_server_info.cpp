#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "../requests/utils.hpp"
#include "sys_server_info.hpp"
#include "utils.hpp"

using json = nlohmann::json;
namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = asio::ip::tcp;

void sysServerInfo(tcp::socket &&socket, http::request<http::string_body> &&req)
{
    try
    {
        std::string target = req.target().to_string();
        size_t pos = target.find('?');
        std::string sysInfo;

        if (pos != std::string::npos)
        {
            std::unordered_map<std::string, std::string> queryParams;
            std::string queryString = target.substr(pos + 1);
            queryParams = getQueryParams(queryString);
            auto it = queryParams.find("sysInfo");
            if (it != queryParams.end())
                sysInfo = it->second;
        }

        json json_res;

        if (sysInfo == "true")
        {
            std::string cpuInfo = getCpuInfo();
            std::string memInfo = getMemoryInfo();
            long uptime = getUpTime();
            int cores = getCoresNum();
            json_res["Server"] = "Healthy";
            json_res["CPU-INFO"] = cpuInfo;
            json_res["MEMORY-INFO"] = memInfo;
            json_res["UP-TIME"] = uptime;
            json_res["CORES"] = cores;
        }
        else
        {
            json_res["Server"] = "Healthy";
        }

        std::string response = json_res.dump();
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.set(http::field::server, "c++ server");
        res.body() = response;
        res.prepare_payload();
        http::write(socket, res);
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Internal Server Error: " << ex.what() << std::endl;
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::content_type, "text/plain");
        res.body() = "Internal Server Error: " + std::string(ex.what());
        res.prepare_payload();
        http::write(socket, res);
    }
}
