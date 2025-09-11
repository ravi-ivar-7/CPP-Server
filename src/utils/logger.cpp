#include "logger.hpp"

// Static member definitions
LogLevel Logger::current_level_ = LogLevel::INFO;
std::ofstream Logger::log_file_;
std::mutex Logger::log_mutex_;
bool Logger::console_output_ = true;
bool Logger::file_output_ = false;
