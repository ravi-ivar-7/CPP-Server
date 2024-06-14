#include <boost/beast.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "./download.hpp"
#include "utils.hpp"
#include "./requests/utils.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace fs = boost::filesystem;
using tcp = boost::asio::ip::tcp;

// TODO: RECONSIDER BELOW CODE
// Sends file from server to client
void downloadFile(tcp::socket &&socket, http::request<http::string_body> &&req)
{
    try
    {
        std::string target = req.target().to_string();
        size_t pos = target.find('?');
        std::string filePath;

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
                std::cerr << "NO FILE PATH PROVIDED" << std::endl;
                throw std::runtime_error("No file path provided");
            }
        }
        else
        {
            std::cerr << "NO QUERY PARAMETERS PROVIDED" << std::endl;
            throw std::runtime_error("No query parameters provided");
        }

        std::string fileContent = readFile(filePath);
        std::string fileName = fs::path(filePath).filename().string();
        std::string mimeType = getFileMimeType(fileName);

        // std::cout << "ROOT DIR: " << fs::current_path() << std::endl;
        // std::cout<<"FILEPATH: " <<filePath<<std::endl;
        // std::cout<<"FILENAME: " <<fileName<<std::endl;

        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "c++");
        res.set(http::field::content_type, mimeType);
        res.set(http::field::content_disposition, "attachment; filename=\"" + fileName + "\"");
        res.body() = fileContent;
        res.prepare_payload();
        http::write(socket, res);
    }
    catch (const std::exception &ex)
    {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, "c++");
        res.set(http::field::content_type, "text/plain");
        res.body() = "File not found: " + std::string(ex.what());
        res.prepare_payload();
        http::write(socket, res);
    }
}
