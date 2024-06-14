#include <boost/algorithm/string.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>


namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;


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

// TODO: CONSIDER BELOW FUNCTIONS

// Function to parse the headers of a part
std::unordered_map<std::string, std::string> parseHeaders(const std::string& headers) {
    std::unordered_map<std::string, std::string> headerMap;
    std::istringstream stream(headers);
    std::string line;
    while (std::getline(stream, line) && line != "\r") {
        auto colon = line.find(':');
        if (colon != std::string::npos) {
            auto key = line.substr(0, colon);
            auto value = line.substr(colon + 1);
            if (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }
            headerMap[key] = value;
        }
    }
    return headerMap;
}

// Function to split the body into parts using the boundary
std::vector<std::string> splitBody(const std::string& body, const std::string& boundary) {
    std::vector<std::string> parts;
    std::string delimiter = "--" + boundary;
    std::string endDelimiter = delimiter + "--";

    size_t pos = 0;
    size_t endPos = 0;
    while ((pos = body.find(delimiter, endPos)) != std::string::npos) {
        endPos = body.find(delimiter, pos + delimiter.length());
        if (endPos == std::string::npos) {
            endPos = body.find(endDelimiter, pos + delimiter.length());
        }
        if (endPos != std::string::npos) {
            parts.push_back(body.substr(pos + delimiter.length() + 2, endPos - pos - delimiter.length() - 4));
        }
    }
    return parts;
}

// Function to parse each part
void parsePart(const std::string& part) {
    // Split headers and body
    auto pos = part.find("\r\n\r\n");
    if (pos != std::string::npos) {
        std::string headers = part.substr(0, pos);
        std::string body = part.substr(pos + 4);

        // Parse headers
        auto headerMap = parseHeaders(headers);

        // Output parsed information
        std::cout << "Part Headers: " << std::endl;
        for (const auto& header : headerMap) {
            std::cout << header.first << ": " << header.second << std::endl;
        }
        std::cout << "Part Body: " << std::endl;
        std::cout << body << std::endl;
    }
}

// Function to handle the HTTP request
void handleRequest(tcp::socket& socket) {
    try {
        // Create a buffer for reading
        beast::flat_buffer buffer;

        // Read the request
        http::request<http::string_body> req;
        http::read(socket, buffer, req);

        // Extract the content type to find the boundary
        auto contentType = req[http::field::content_type].to_string();
        std::string boundary;
        auto pos = contentType.find("boundary=");
        if (pos != std::string::npos) {
            boundary = contentType.substr(pos + 9);
        } else {
            throw std::runtime_error("Boundary not found in Content-Type");
        }

        // Split the body into parts
        auto parts = splitBody(req.body(), boundary);

        // Parse each part
        for (const auto& part : parts) {
            parsePart(part);
        }

        // Send a simple response
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Boost.Beast");
        res.set(http::field::content_type, "text/plain");
        res.body() = "Parts parsed successfully!";
        res.prepare_payload();
        http::write(socket, res);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        http::response<http::string_body> res{http::status::internal_server_error, 11};
        res.set(http::field::content_type, "text/plain");
        res.body() = "Internal Server Error: " + std::string(e.what());
        res.prepare_payload();
        http::write(socket, res);
    }
}

