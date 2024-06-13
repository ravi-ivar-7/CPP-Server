#ifndef SRC_UTILS_COMMON_HPP
#define SRC_UTILS_COMMON_HPP

#include <string>
#include <unordered_map>
#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

// Function declarations
std::unordered_map<std::string, std::string> getQueryParams(const std::string &query);
void notFound(tcp::socket &&socket, http::request<http::string_body> &&req);
std::string readFile(const std::string &filePath);

#endif // SRC_UTILS_COMMON_HPP