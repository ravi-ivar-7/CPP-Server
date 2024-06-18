#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <mutex>

#include "stream_text.hpp"
#include "../requests/utils.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = boost::asio::ip::tcp;

void StreamText::onMessage(std::shared_ptr<websocket::stream<tcp::socket>> ws, const beast::http::request<beast::http::string_body>& req, beast::flat_buffer& buffer) {
    try {
        std::string message = beast::buffers_to_string(buffer.data());

        std::string userId = getUserId(req);

        // Broadcasting the incoming message to all active sessions
        std::lock_guard<std::mutex> lock(sessionsMutex);
        for (const auto& session : activeSessions) {
            std::string writeMessage = userId + " : " + message;
            doWrite(session.second, std::move(writeMessage));
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in onMessage: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in onMessage" << std::endl;
    }
}


bool StreamText::authenticate(const beast::http::request<beast::http::string_body> &req)
{
    try {
        std::string target = req.target().to_string();
        size_t pos = target.find('?');
        if (pos != std::string::npos)
        {
            std::unordered_map<std::string, std::string> queryParams;
            std::string queryString = target.substr(pos + 1);
            queryParams = getQueryParams(queryString);
            auto it = queryParams.find("authToken");
            if (it != queryParams.end() && it->second == "StreamTextSecretToken")
                return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in authenticate: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in authenticate" << std::endl;
    }
    return false;
}

std::string StreamText::getUserId(const beast::http::request<beast::http::string_body> &req)
{
    try {
        std::string target = req.target().to_string();
        size_t pos = target.find('?');

        if (pos != std::string::npos)
        {
            std::unordered_map<std::string, std::string> queryParams;
            std::string queryString = target.substr(pos + 1);
            queryParams = getQueryParams(queryString);
            auto it = queryParams.find("userId");
            if (it != queryParams.end())
                return it->second;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in getUserId: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in getUserId" << std::endl;
    }
    return "";
}
