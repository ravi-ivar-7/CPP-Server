#include <nlohmann/json.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

#include "bcrypt.h"
#include "authenticate.hpp"
#include "../system/log.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;
using json = nlohmann::json;

// Sample credentials
std::unordered_map<std::string, std::string> credentials = {
    {"user", bcrypt::generateHash("userpassword")},
    {"admin", bcrypt::generateHash("adminpassword")}
};

void authenticate(tcp::socket &&socket, http::request<http::string_body> &&req)
{
    try
    {
        const std::string &body = req.body();
        json parsedBody = json::parse(body);

        std::string username = parsedBody.at("username").get<std::string>();
        std::string password = parsedBody.at("password").get<std::string>();

        bool isAuthenticated = false;
        auto it = credentials.find(username);
        if (it != credentials.end())
        {
            isAuthenticated = bcrypt::validatePassword(password, it->second);
        }

        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "c++");
        res.set(http::field::content_type, "text/plain");

        if (isAuthenticated)
        {
            res.body() = "Authentication successful!";
        }
        else
        {
            res.body() = "Authentication failed!";
        }

        res.prepare_payload();
        http::write(socket, res);
    }
    catch (const json::parse_error &e)
    {
        std::string errorMsg = "JSON Parsing error: " + std::string(e.what());
        saveLog("ERROR", errorMsg);
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, "c++");
        res.set(http::field::content_type, "text/plain");
        res.body() = errorMsg;
        res.prepare_payload();
        http::write(socket, res);
    }
    catch (const json::type_error &e)
    {
        std::string errorMsg = "JSON Data access error: " + std::string(e.what());
        saveLog("ERROR", errorMsg);
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, "c++");
        res.set(http::field::content_type, "text/plain");
        res.body() = errorMsg;
        res.prepare_payload();
        http::write(socket, res);
    }
    catch (const std::exception &e)
    {
        std::string errorMsg = "Server error: " + std::string(e.what());
        saveLog("ERROR", errorMsg);
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, "c++");
        res.set(http::field::content_type, "text/plain");
        res.body() = errorMsg;
        res.prepare_payload();
        http::write(socket, res);
    }
}
