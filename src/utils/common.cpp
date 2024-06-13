#include "common.hpp"
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

std::unordered_map<std::string, std::string> getQueryParams(const std::string &query)
{
    std::unordered_map<std::string, std::string> keyValues;
    std::vector<std::string> pairs;
    boost::algorithm::split(pairs, query, boost::is_any_of("&"));
    for (const auto &pair : pairs)
    {
        std::vector<std::string> thisKeyValue;
        boost::algorithm::split(thisKeyValue, pair, boost::is_any_of("="));
        if (thisKeyValue.size() == 2)
        {
            keyValues[thisKeyValue[0]] = thisKeyValue[1];
        }
    }
    return keyValues;
}

std::string readFile(const std::string &filePath) // file path should be from root dir
{
    // std::cout << "READ FILE PATH: " << fs::current_path() << std::endl; // this gives root dir not this file dir
    // std::cout << "FILE PATH: " << filePath << std::endl;
    try
    {
        std::ifstream reqFile(filePath, std::ios::in | std::ios::binary);
        if (!reqFile.is_open())
            throw std::runtime_error("Failed to open file: " + filePath);

        reqFile.seekg(0, std::ios::end);        // moves the file pointer to a specified position(end)
        std::size_t fileSize = reqFile.tellg(); // current position of the file pointer(file size)
        reqFile.seekg(0, std::ios::beg);        // file pointer back to the beginning

        std::vector<char> buffer(fileSize);
        reqFile.read(buffer.data(), fileSize);
        reqFile.close();

        return std::string(buffer.begin(), buffer.end());
    }
    catch (const std::exception &ex)
    {
        std::cerr << "ERROR READING FILE: " << ex.what() << std::endl;
        throw;
    }
}

void notFound(tcp::socket &&socket, http::request<http::string_body> &&req)
{
    http::response<http::string_body> res{http::status::not_found, req.version()};
    res.set(http::field::server, "Beast");
    res.set(http::field::content_type, "text/plain");
    res.body() = "404 Not Found";
    res.prepare_payload();
    http::write(socket, res);
}
