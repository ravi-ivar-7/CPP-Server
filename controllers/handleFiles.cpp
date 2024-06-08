#include "../includes/handleFiles.hpp"
#include <boost/beast.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

// For file-system link against the Boost Filesystem library
// g++ -o server server.cpp -I/usr/include -lboost_system -lboost_filesystem

namespace beast = boost::beast;
namespace http = beast::http;
namespace fs = boost::filesystem;
using tcp = boost::asio::ip::tcp;

// Handling files uploaded by users
std::string getFilenameFromHeader(const std::string &header)
{
    std::regex filenameRegex("filename=\"([^\"]+)\"");
    std::smatch matches;
    if (std::regex_search(header, matches, filenameRegex) && matches.size() > 1)
    {
        return matches[1].str();
    }
    return "uploaded_file"; // Default filename if not found
}

// ISSUE WITH FILE NAME AND EXTENSION
void handleFileUpload(tcp::socket&& socket, http::request<http::string_body>&& req)
{
    // Extract the boundary from the Content-Type header
    std::string contentType(req[http::field::content_type].begin(), req[http::field::content_type].end());
    std::string boundary = "--" + contentType.substr(contentType.find("boundary=") + 9);

    // Extract file content and headers
    std::string body = req.body();
    size_t start = body.find(boundary) + boundary.length() + 2; // skip boundary and \r\n
    size_t end = body.find(boundary, start) - 2;                // go back before \r\n
    std::string filePart = body.substr(start, end - start);

    // Extract headers from file part
    size_t headerEnd = filePart.find("\r\n\r\n");
    std::string headers = filePart.substr(0, headerEnd);
    std::string fileContent = filePart.substr(headerEnd + 4);

    // Get filename from headers
    std::istringstream headerStream(headers);
    std::string line;
    std::string filename = "uploaded_file";
    while (std::getline(headerStream, line))
    {
        if (line.find("Content-Disposition") != std::string::npos)
        {
            filename = getFilenameFromHeader(line);
        }
    }

    // Save the file
    std::ofstream ofs(filename, std::ios::binary);
    ofs << fileContent;
    ofs.close();

    // Send the response
    http::response<http::string_body> res{http::status::ok, req.version()};
    res.set(http::field::server, "c++");
    res.set(http::field::content_type, "text/plain");
    res.body() = "File uploaded successfully as " + filename;
    res.prepare_payload();
    http::write(socket, res);
}

// Handling files downloaded by uses
bool endsWith(const std::string &str, const std::string &suffix)
{
    if (str.length() < suffix.length())
        return false;
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

std::string getFileMimeType(const std::string &path)
{
    // Define a map of file extensions to MIME types
    static const std::map<std::string, std::string> mimeTypes = {
        {".html", "text/html"},
        {".htm", "text/html"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".png", "image/png"},
        {".gif", "image/gif"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".json", "application/json"},
        {".pdf", "application/pdf"},
        {".txt", "text/plain"},
        {".xml", "application/xml"},
        {".csv", "text/csv"},
        {".zip", "application/zip"},
        {".tar", "application/x-tar"},
        {".rar", "application/x-rar-compressed"},
        {".7z", "application/x-7z-compressed"},
        {".cpp", "text/x-c++src"},
        {".hpp", "text/x-c++hdr"},
        {".c", "text/x-csrc"},
        {".h", "text/x-chdr"}
    };

    // Find the file extension
    std::string::size_type dot_pos = path.rfind('.');
    if (dot_pos != std::string::npos)
    {
        std::string extension = path.substr(dot_pos);
        auto it = mimeTypes.find(extension);
        if (it != mimeTypes.end())
        {
            return it->second;
        }
    }

    // Default MIME type if no match is found
    return "application/octet-stream";
}

void handleFileDownload(tcp::socket &&socket, http::request<http::string_body> &&req)
{
    std::string filePath = "./config/.env"; // becarefull of this path

    // std::cout << "Current working directory: " << fs::current_path() << std::endl;// this will give root dir not this file dir

    if (fs::exists(filePath))
    {
        // Read file content
        std::ifstream ifs(filePath, std::ios::binary);

        if (!ifs.is_open())
        {
            // File could not be opened
            http::response<http::string_body> res{http::status::internal_server_error, req.version()};
            res.set(http::field::server, "Beast");
            res.set(http::field::content_type, "text/plain");
            res.body() = "Internal Server Error: Failed to open file";
            res.prepare_payload();
            http::write(socket, res);
            return;
        }

        std::ostringstream oss;
        oss << ifs.rdbuf();
        std::string fileContent = oss.str();

        // Determine the MIME type of the file
        std::string mimeType = getFileMimeType(filePath);

        // Get the filename from the file path
        std::string fileName = fs::path(filePath).filename().string();

        // Send the file content
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, mimeType);
        res.set(http::field::content_disposition, "attachment; filename=\"" + fileName + "\"");
        res.body() = fileContent;
        res.prepare_payload();
        http::write(socket, res);
    }
    else
    {
        // Respond with a 404 Not Found if file does not exist
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "text/plain");
        res.body() = "File not found";
        res.prepare_payload();
        http::write(socket, res);
    }
}

