#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <mutex>

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
    FATAL = 4
};

class Logger {
private:
    static LogLevel current_level_;
    static std::ofstream log_file_;
    static std::mutex log_mutex_;
    static bool console_output_;
    static bool file_output_;

    static std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

    static std::string levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO:  return "INFO ";
            case LogLevel::WARN:  return "WARN ";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::FATAL: return "FATAL";
            default: return "UNKNOWN";
        }
    }

    static void writeLog(LogLevel level, const std::string& message, 
                        const std::string& file = "", int line = 0) {
        if (level < current_level_) return;

        std::lock_guard<std::mutex> lock(log_mutex_);
        
        std::stringstream log_entry;
        log_entry << "[" << getCurrentTimestamp() << "] "
                  << "[" << levelToString(level) << "] ";
        
        if (!file.empty() && line > 0) {
            log_entry << "[" << file << ":" << line << "] ";
        }
        
        log_entry << message;

        if (console_output_) {
            if (level >= LogLevel::ERROR) {
                std::cerr << log_entry.str() << std::endl;
            } else {
                std::cout << log_entry.str() << std::endl;
            }
        }

        if (file_output_ && log_file_.is_open()) {
            log_file_ << log_entry.str() << std::endl;
            log_file_.flush();
        }
    }

public:
    static void init(const std::string& log_file_path = "", 
                    LogLevel level = LogLevel::INFO,
                    bool console = true, bool file = false) {
        current_level_ = level;
        console_output_ = console;
        file_output_ = file;

        if (file_output_ && !log_file_path.empty()) {
            log_file_.open(log_file_path, std::ios::app);
            if (!log_file_.is_open()) {
                std::cerr << "Failed to open log file: " << log_file_path << std::endl;
                file_output_ = false;
            }
        }
    }

    static void setLevel(LogLevel level) {
        current_level_ = level;
    }

    static void debug(const std::string& message, const std::string& file = "", int line = 0) {
        writeLog(LogLevel::DEBUG, message, file, line);
    }

    static void info(const std::string& message, const std::string& file = "", int line = 0) {
        writeLog(LogLevel::INFO, message, file, line);
    }

    static void warn(const std::string& message, const std::string& file = "", int line = 0) {
        writeLog(LogLevel::WARN, message, file, line);
    }

    static void error(const std::string& message, const std::string& file = "", int line = 0) {
        writeLog(LogLevel::ERROR, message, file, line);
    }

    static void fatal(const std::string& message, const std::string& file = "", int line = 0) {
        writeLog(LogLevel::FATAL, message, file, line);
    }

    static void cleanup() {
        if (log_file_.is_open()) {
            log_file_.close();
        }
    }
};

// Convenience macros for logging with file and line info
#define LOG_DEBUG(msg) Logger::debug(msg, __FILE__, __LINE__)
#define LOG_INFO(msg) Logger::info(msg, __FILE__, __LINE__)
#define LOG_WARN(msg) Logger::warn(msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) Logger::error(msg, __FILE__, __LINE__)
#define LOG_FATAL(msg) Logger::fatal(msg, __FILE__, __LINE__)
