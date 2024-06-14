#ifndef SRC_FILES_UTILS_HPP
#define SRC_FILES_UTILS_HPP

#include <string>

void writeFile(const std::string &filePath, const std::string &data);
std::string readFile(const std::string &filePath);
std::string getFileName(const std::string &contentDisposition);
std::string getFileExtension(const std::string &filePath);
std::string getFileMimeType(const std::string &fileName);

#endif // SRC_FILES_UTILS_HPP