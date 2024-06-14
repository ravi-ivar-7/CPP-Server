#ifndef SRC_REQUESTS_UTILS_HPP
#define SRC_REQUESTS_UTILS_HPP

#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::string> getQueryParams(const std::string &query);

#endif // SRC_REQUESTS_UTILS_HPP