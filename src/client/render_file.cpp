#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include "../files/utils.hpp"
#include "../requests/utils.hpp"
#include "../system/log.hpp"
#include "render_file.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

void renderFile(tcp::socket &&socket, http::request<http::string_body> &&req)
{
    std::string filePath;
    std::string target = std::string(req.target());
    size_t pos = target.find('?');
    if (pos != std::string::npos)
    {
        std::unordered_map<std::string, std::string> queryParams;
        std::string queryString = target.substr(pos + 1);
        queryParams = getQueryParams(queryString);
        auto it = queryParams.find("filePath");
        if (it != queryParams.end())
            filePath = it->second;
        else
        {
            throw std::runtime_error("NO FILE PATH PROVIDED");
        }
    }

    try
    {
        std::string fileContent = readFile(filePath);
        
        // Determine content type based on file extension
        std::string contentType = "text/plain";
        if (filePath.length() >= 3 && filePath.substr(filePath.length() - 3) == ".md")
            contentType = "text/markdown";
        else if (filePath.length() >= 5 && filePath.substr(filePath.length() - 5) == ".html")
            contentType = "text/html";
        else if (filePath.length() >= 4 && filePath.substr(filePath.length() - 4) == ".css")
            contentType = "text/css";
        else if (filePath.length() >= 3 && filePath.substr(filePath.length() - 3) == ".js")
            contentType = "application/javascript";
        else if (filePath.length() >= 5 && filePath.substr(filePath.length() - 5) == ".json")
            contentType = "application/json";
        
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::content_type, contentType);
        res.set(http::field::access_control_allow_origin, "*");
        res.body() = fileContent;
        res.prepare_payload();
        http::write(socket, res);
    }
    catch (const std::exception &e)
    {
        saveLog("ERROR", std::string(e.what()));

        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::content_type, "text/plain");
        res.body() = "Error reading file: " + std::string(e.what());
        res.prepare_payload();
        http::write(socket, res);
    }
}
