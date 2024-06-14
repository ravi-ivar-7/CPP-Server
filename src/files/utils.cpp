#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <mutex>

namespace fs = boost::filesystem;

std::mutex writeFileMutex;

void writeFile(const std::string &filePath, const std::string &data)
{
    try
    {
        fs::path path(filePath);
        if (!fs::exists(path.parent_path()))
        {
            fs::create_directories(path.parent_path());
        }

        std::lock_guard<std::mutex> lock(writeFileMutex);

        std::ofstream outFile(path.string(), std::ios::app);
        if (!outFile)
            throw std::ios_base::failure("Failed to open the file for writing.");

        outFile << data;
        outFile.close();
        std::cout << "Data written to file successfully." << std::endl;
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "General error: " << e.what() << std::endl;
    }
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

        reqFile.seekg(0, std::ios::end);        // moves the file pointer to a specified position (end)
        std::size_t fileSize = reqFile.tellg(); // current position of the file pointer (file size)
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

std::string getFileExtension(const std::string &filePath)
{
    std::vector<std::string> possibleExtensions;
    boost::split(possibleExtensions, filePath, boost::is_any_of("."));

    if (possibleExtensions.size() > 1 && !possibleExtensions.back().empty())
    {
        return possibleExtensions.back();
    }
    return "";
}

std::string getFileMimeType(const std::string &fileName)
{
    std::unordered_map<std::string, std::string> fileMimeType = {
        {"html", "text/html"},
        {"htm", "text/html"},
        {"jpg", "image/jpeg"},
        {"jpeg", "image/jpeg"},
        {"png", "image/png"},
        {"gif", "image/gif"},
        {"css", "text/css"},
        {"js", "application/javascript"},
        {"json", "application/json"},
        {"pdf", "application/pdf"},
        {"txt", "text/plain"},
        {"xml", "application/xml"},
        {"csv", "text/csv"},
        {"zip", "application/zip"},
        {"tar", "application/x-tar"},
        {"rar", "application/x-rar-compressed"},
        {"7z", "application/x-7z-compressed"},
        {"cpp", "text/x-c++src"},
        {"hpp", "text/x-c++hdr"},
        {"c", "text/x-csrc"},
        {"h", "text/x-chdr"}};

    std::string extension = getFileExtension(fileName);
    auto it = fileMimeType.find(extension);
    if (it != fileMimeType.end())
    {
        return it->second;
    }
    return "application/octet-stream"; // Default MIME type for unknown files
}
