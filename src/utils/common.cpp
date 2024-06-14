#include "common.hpp"
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;





void notFound(tcp::socket &&socket, http::request<http::string_body> &&req)
{
    http::response<http::string_body> res{http::status::not_found, req.version()};
    res.set(http::field::server, "Beast");
    res.set(http::field::content_type, "text/plain");
    res.body() = "404 Not Found";
    res.prepare_payload();
    http::write(socket, res);
}
