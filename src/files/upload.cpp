#include <boost/beast.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "./upload.hpp"
#include "../utils/common.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace fs = boost::filesystem;
using tcp = boost::asio::ip::tcp;

std::string getFileName(const std::string &contentDisposition)
{
    std::string fileName;
    size_t pos = contentDisposition.find("filename=");
    if (pos != std::string::npos)
    {
        fileName = contentDisposition.substr(pos + 9);
        if (fileName.front() == '"' && fileName.back() == '"')
        { // remove any surrounding quotes
            fileName = fileName.substr(1, fileName.size() - 2);
        }
    }
    return fileName;
}

void uploadFile(tcp::socket &&socket, http::request<http::string_body> &&req)
{
    try
    {
        auto it = req.find("Content-Disposition");
        auto& headers = req.base();

        auto cdheader = headers.find(http::field::content_disposition);
        
        // std::cout << "HTTP Headers:\n";
        // for (auto& header : headers)
        // {
        //     std::cout << header.name_string() << ": " << header.value() << "\n";
        // }
        // if (cdheader == headers.end())
        // {
        //     throw std::runtime_error("Missing Content-Disposition header");
        // }

        std::string contentDisposition = cdheader->value().to_string();
        std::string fileName = getFileName(contentDisposition);


        std::string contentType = req[http::field::content_type].to_string();
        std::string boundary = "--" + contentType.substr(contentType.find("boundary=") + 9);
        std::string body = req.body();

        size_t start = body.find(boundary) + boundary.length() + 2; // skip boundary and \r\n
        size_t end = body.find(boundary, start) - 2;                // go back before \r\n
        std::string filePart = body.substr(start, end - start);

        size_t headerEnd = filePart.find("\r\n\r\n");
        std::string fileContent = filePart.substr(headerEnd + 4);
        
        if (fileName.empty())
        {
            fileName = "downloaded.txt";
        }

        fs::path output_dir = fs::current_path() / "assets/downloads";
        fs::create_directories(output_dir);
        fs::path output_file = output_dir / fileName;

        std::ofstream outfile(output_file.string(), std::ios::binary);
        if (!outfile.is_open())
        {
            std::cerr << "Error: Could not open file for writing: " << output_file << std::endl;
            throw std::runtime_error("Could not open file for writing");
        }

        outfile.write(fileContent.data(), fileContent.size());
        outfile.close();

        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "c++");
        res.set(http::field::content_type, "text/plain");
        res.body() = "Successfully uploaded file.";
        res.prepare_payload();
        http::write(socket, res);
    }
    catch (const std::exception &ex)
    {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, "c++");
        res.set(http::field::content_type, "text/plain");
        res.body() = "Error occurred: " + std::string(ex.what());
        res.prepare_payload();
        http::write(socket, res);
    }
}

