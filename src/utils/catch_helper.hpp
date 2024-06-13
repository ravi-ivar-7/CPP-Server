#ifndef ERROR_HANDLING_HPP
#define ERROR_HANDLING_HPP

#include <boost/beast.hpp>
#include <iostream> 
#include <stdexcept>

namespace http = boost::beast::http;

// Function to handle runtime errors and send HTTP response
void handle_runtime_error(boost::beast::tcp_stream& stream, const http::request<http::string_body>& req, const std::runtime_error& ex) {
    std::cerr << "Runtime error occurred: " << ex.what() << std::endl;

    // Prepare HTTP response
    http::response<http::string_body> res{http::status::internal_server_error, req.version()};
    res.set(http::field::content_type, "text/plain");
    res.body() = "Internal Server Error: " + std::string(ex.what());
    res.prepare_payload();

    // Send the HTTP response
    http::write(stream, res);
}

// Function to handle logic errors and send HTTP response
void handle_logic_error(boost::beast::tcp_stream& stream, const http::request<http::string_body>& req, const std::logic_error& ex) {
    std::cerr << "Logic error occurred: " << ex.what() << std::endl;

    // Prepare HTTP response
    http::response<http::string_body> res{http::status::bad_request, req.version()};
    res.set(http::field::content_type, "text/plain");
    res.body() = "Bad Request: " + std::string(ex.what());
    res.prepare_payload();

    // Send the HTTP response
    http::write(stream, res);
}

// Function to handle Boost.System errors and send HTTP response
void handle_boost_system_error(boost::beast::tcp_stream& stream, const http::request<http::string_body>& req, const boost::system::system_error& ex) {
    std::cerr << "Boost.System error occurred: " << ex.what() << " (code: " << ex.code() << ")" << std::endl;

    // Prepare HTTP response
    http::response<http::string_body> res{http::status::internal_server_error, req.version()};
    res.set(http::field::content_type, "text/plain");
    res.body() = "Internal Server Error: " + std::string(ex.what()) + " (code: " + std::to_string(ex.code().value()) + ")";
    res.prepare_payload();

    // Send the HTTP response
    http::write(stream, res);
}

// Function to handle unknown exceptions and send HTTP response
void handle_unknown_exception(boost::beast::tcp_stream& stream, const http::request<http::string_body>& req) {
    std::cerr << "Unknown exception occurred" << std::endl;

    // Prepare HTTP response
    http::response<http::string_body> res{http::status::internal_server_error, req.version()};
    res.set(http::field::content_type, "text/plain");
    res.body() = "Internal Server Error: Unknown Exception";
    res.prepare_payload();

    // Send the HTTP response
    http::write(stream, res);
}

#endif // ERROR_HANDLING_HPP
