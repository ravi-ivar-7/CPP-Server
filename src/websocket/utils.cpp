#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <mutex>
#include "utils.hpp"
#include <iostream>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = boost::asio::ip::tcp;

std::unordered_map<std::string, std::shared_ptr<websocket::stream<tcp::socket>>> WSUtils::activeSessions;
std::mutex WSUtils::sessionsMutex;

void WSUtils::wsRequest(tcp::socket &&socket, beast::http::request<beast::http::string_body> req) {
    try {
        if (!authenticate(req)) {
            std::cerr << "Authentication failed" << std::endl;
            return;
        }
        std::string userId = getUserId(req);

        if(activeSessions.find(userId) != activeSessions.end())
        {
            std::cerr<<"Active session exists."<<std::endl;
            return;
        }

        auto ws = std::make_shared<websocket::stream<tcp::socket>>(std::move(socket));
        {
            std::lock_guard<std::mutex> lock(sessionsMutex);
            activeSessions[userId] = ws;
        }
        
        doAccept(ws, std::move(req));
    } catch (const std::exception& e) {
        std::cerr << "Exception in wsRequest: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in wsRequest" << std::endl;
    }
}

void WSUtils::doAccept(std::shared_ptr<websocket::stream<tcp::socket>> ws, beast::http::request<beast::http::string_body> req) {
    try {
        ws->async_accept(req, [this, ws, req](beast::error_code ec) mutable {
            if (ec) {
                std::cerr << "Error accepting WebSocket: " << ec.message() << std::endl;
                return;
            }
            auto buffer = std::make_shared<beast::flat_buffer>();
            doRead(ws, std::move(req), buffer);
        });
    } catch (const std::exception& e) {
        std::cerr << "Exception in doAccept: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in doAccept" << std::endl;
    }
}

void WSUtils::doRead(std::shared_ptr<websocket::stream<tcp::socket>> ws, beast::http::request<beast::http::string_body> req, std::shared_ptr<beast::flat_buffer> buffer) {
    try {
        ws->async_read(*buffer, [this, ws, buffer, req = std::move(req)](beast::error_code ec, std::size_t bytes_transferred) mutable {
            if (ec) {
                std::cerr << "ERROR READING WS: " << ec.message() << std::endl;

                // Remove session on error
                std::lock_guard<std::mutex> lock(sessionsMutex);
                auto it = std::find_if(activeSessions.begin(), activeSessions.end(),
                                       [&ws](const auto& pair) { return pair.second == ws; });
                if (it != activeSessions.end()) {
                    activeSessions.erase(it);
                }

                return;
            }
            onMessage(ws, req, *buffer);
            buffer->consume(bytes_transferred);
            doRead(ws, std::move(req), buffer); 
        });
    } catch (const std::exception& e) {
        std::cerr << "Exception in doRead: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in doRead" << std::endl;
    }
}



void WSUtils::doWrite(std::shared_ptr<websocket::stream<tcp::socket>> ws, std::string message) {
    try {
        ws->text(ws->got_text());
        ws->async_write(beast::net::buffer(message), [ws](beast::error_code ec, std::size_t/*bytes_transferred*/ ) {
            if (ec) {
                std::cerr << "ERROR WRITING WS: " << ec.message() << std::endl;
            }
        });
    } catch (const std::exception& e) {
        std::cerr << "Exception in doWrite: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in doWrite" << std::endl;
    }
}

bool WSUtils::authenticate(const beast::http::request<beast::http::string_body>& req) {
    auto authHeader = req.find("Authorization");
    if (authHeader != req.end() && authHeader->value() == "authbearer1234") {
        return true;
    }
    return false;
}

std::string WSUtils::getUserId(const beast::http::request<beast::http::string_body>& req) {
    auto idHeader = req.find("X-Client-ID");
    if (idHeader != req.end()) {
        return idHeader->value().to_string();
    }
    return "";
}
