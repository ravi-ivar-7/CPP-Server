#ifndef SRC_SECURITY_ENCRYPTION_DECRYPTION_HPP
#define SRC_SECURITY_ENCRYPTION_DECRYPTION_HPP

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

// Function prototypes
void encryptData(tcp::socket &&socket, http::request<http::string_body> &&req);
void decryptData(tcp::socket &&socket, http::request<http::string_body> &&req);

#endif // SRC_SECURITY_ENCRYPTION_DECRYPTION_HPP
