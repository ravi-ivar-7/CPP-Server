#ifndef SRC_SYSTEM_UTILS_HPP
#define SRC_SYSTEM_UTILS_HPP

#include <string>

std::string getCpuInfo();
std::string getMemoryInfo();
long getUpTime();
int getCoresNum();

#endif // SRC_SYSTEM_UTILS_HPP