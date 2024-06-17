#include "utils.hpp"
#include <iostream>

void WSUtils::wsRequest(tcp::socket &&socket, beast::http::request<beast::http::string_body> req) {
    auto ws = std::make_shared<websocket::stream<tcp::socket>>(std::move(socket));
    doAccept(ws, std::move(req));
}

void WSUtils::doAccept(std::shared_ptr<websocket::stream<tcp::socket>> ws, beast::http::request<beast::http::string_body> req) {
    ws->async_accept(req, [this, ws](beast::error_code ec) {
        if (ec) {
            std::cerr << "ERROR ACCEPTING WS: " << ec.message() << std::endl;
            return;
        }
        auto buffer = std::make_shared<beast::flat_buffer>();
        doRead(ws, *buffer);
    });
}

void WSUtils::doRead(std::shared_ptr<websocket::stream<tcp::socket>> ws, beast::flat_buffer &buffer) {
    ws->async_read(buffer, [this, ws, &buffer](beast::error_code ec, std::size_t bytes_transferred) {
        if (ec) {
            std::cerr << "ERROR READING WS: " << ec.message() << std::endl;
            return;
        }
        auto message = beast::buffers_to_string(buffer.data());
        onMessage(ws, message);
        buffer.consume(bytes_transferred);
        doRead(ws, buffer);
    });
}

void WSUtils::doWrite(std::shared_ptr<websocket::stream<tcp::socket>> ws, beast::flat_buffer &buffer, std::string message) {
    ws->text(ws->got_text());
    ws->async_write(beast::net::buffer(message), [ws](beast::error_code ec, std::size_t bytes_transferred) {
        if (ec) {
            std::cerr << "ERROR WRITING WS: " << ec.message() << std::endl;
        }
    });
}
