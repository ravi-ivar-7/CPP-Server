#ifndef SRC_STREAM_STREAM_TEXT_HPP
#define STREAM_TEXT_HPP

#include "../websocket/utils.hpp"
#include <string>
#include <memory>

class StreamText : public WSUtils {
public:
    void onMessage(std::shared_ptr<websocket::stream<tcp::socket>> ws, std::string message) override;
};

#endif // SRC_STREAM_STREAM_TEXT_HPP

