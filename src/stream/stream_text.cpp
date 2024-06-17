
#include "stream_text.hpp"
#include <iostream>

void StreamText::onMessage(std::shared_ptr<websocket::stream<tcp::socket>> ws, std::string message) {
    std::cout << "Received: " << message << std::endl;

    message += " hello";
    auto buffer = std::make_shared<beast::flat_buffer>();
    doWrite(ws, *buffer, message);
}
