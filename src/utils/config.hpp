#pragma once

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

class Config {
private:
    static std::map<std::string, std::string> config_map_;
    static bool initialized_;

    static void loadFromFile(const std::string& file_path) {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "Warning: Could not open config file: " << file_path << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Skip empty lines and comments
            if (line.empty() || line[0] == '#') continue;
            
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                
                // Trim whitespace
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                
                config_map_[key] = value;
            }
        }
    }

    static void loadFromEnvironment() {
        // Load common environment variables
        const char* env_vars[] = {
            "SERVER_PORT", "LOG_LEVEL", "LOG_FILE", "DB_PATH", 
            "RSA_PRIVATE_KEY", "RSA_PUBLIC_KEY", "BCRYPT_ROUNDS"
        };
        
        for (const char* var : env_vars) {
            const char* value = std::getenv(var);
            if (value) {
                config_map_[var] = std::string(value);
            }
        }
    }

public:
    static void init(const std::string& config_file = "config/.env") {
        if (initialized_) return;
        
        // Set defaults
        config_map_["SERVER_PORT"] = "8080";
        config_map_["LOG_LEVEL"] = "INFO";
        config_map_["LOG_FILE"] = "logs/server.log";
        config_map_["DB_PATH"] = "data/server.db";
        config_map_["RSA_PRIVATE_KEY"] = "config/privateKey.pem";
        config_map_["RSA_PUBLIC_KEY"] = "config/publicKey.pem";
        config_map_["BCRYPT_ROUNDS"] = "12";
        config_map_["THREAD_POOL_SIZE"] = "4";
        
        // Load from file (overrides defaults)
        loadFromFile(config_file);
        
        // Load from environment (overrides file)
        loadFromEnvironment();
        
        initialized_ = true;
    }

    static std::string get(const std::string& key, const std::string& default_value = "") {
        if (!initialized_) init();
        
        auto it = config_map_.find(key);
        return (it != config_map_.end()) ? it->second : default_value;
    }

    static int getInt(const std::string& key, int default_value = 0) {
        std::string value = get(key);
        if (value.empty()) return default_value;
        
        try {
            return std::stoi(value);
        } catch (const std::exception&) {
            return default_value;
        }
    }

    static bool getBool(const std::string& key, bool default_value = false) {
        std::string value = get(key);
        if (value.empty()) return default_value;
        
        // Convert to lowercase for comparison
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        return (value == "true" || value == "1" || value == "yes" || value == "on");
    }

    static void set(const std::string& key, const std::string& value) {
        if (!initialized_) init();
        config_map_[key] = value;
    }

    static void printAll() {
        if (!initialized_) init();
        
        std::cout << "Configuration:" << std::endl;
        for (const auto& pair : config_map_) {
            std::cout << "  " << pair.first << " = " << pair.second << std::endl;
        }
    }
};

// Static member declarations only - definitions in .cpp file
