#ifndef SRC_SECURITY_UTILS_HPP
#define SRC_SECURITY_UTILS_HPP


#include <string>


void handleOpenSSLErrors();
void initializeOpenSSLErrors();
void cleanupOpenSSLErrors();

std::string rsaEncrypt(const std::string& data, const std::string &publicKey);
std::string rsaDecrypt(const std::string& encryptedData,const std::string &privateKey);

#endif // SRC_SECURITY_UTILS_HPP