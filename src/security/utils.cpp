#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <boost/beast/core/detail/base64.hpp>
#include <string>
#include <iostream>
#include <stdexcept>

#include "utils.hpp"

void handleOpenSSLErrors() {
    char err[130];
    ERR_error_string_n(ERR_get_error(), err, sizeof(err));
    throw std::runtime_error(err);
}

void initializeOpenSSLErrors() {
    ERR_load_crypto_strings();
}

void cleanupOpenSSLErrors() {
    ERR_free_strings();
}

RSA* readPublicKeyFromBuffer(const std::string& keyStr) {
    RSA* rsa = nullptr;
    BIO* keybio = BIO_new_mem_buf(keyStr.c_str(), -1);
    if (keybio == nullptr) {
        handleOpenSSLErrors();
    }

    rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, nullptr, nullptr);
    if (rsa == nullptr) {
        BIO_free(keybio);
        handleOpenSSLErrors();
    }

    BIO_free(keybio);
    return rsa;
}

RSA* readPrivateKeyFromBuffer(const std::string& keyStr) {
    RSA* rsa = nullptr;
    BIO* keybio = BIO_new_mem_buf(keyStr.c_str(), -1);
    if (keybio == nullptr) {
        handleOpenSSLErrors();
    }

    rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, nullptr, nullptr);
    if (rsa == nullptr) {
        BIO_free(keybio);
        handleOpenSSLErrors();
    }

    BIO_free(keybio);
    return rsa;
}

std::string rsaEncrypt(const std::string& data, const std::string &publicKeyStr) {
    try {
        RSA* rsa = readPublicKeyFromBuffer(publicKeyStr);
        int rsaLen = RSA_size(rsa);
        unsigned char* encrypted = new unsigned char[rsaLen];

        int result = RSA_public_encrypt(data.size(), reinterpret_cast<const unsigned char*>(data.c_str()), encrypted, rsa, RSA_PKCS1_OAEP_PADDING);
        if (result == -1) {
            delete[] encrypted;
            RSA_free(rsa);
            handleOpenSSLErrors();
        }

        std::string encryptedData(reinterpret_cast<char*>(encrypted), result); // this is in binary form

        // Calculate the size of the encoded data
        std::string encryptedEncodedData;
        encryptedEncodedData.resize(boost::beast::detail::base64::encoded_size(encryptedData.size()));

        // Encode the binary data using Base64
        const auto encoded = boost::beast::detail::base64::encode(&encryptedEncodedData[0], encryptedData.data(), encryptedData.size());
        encryptedEncodedData.resize(encoded);

        delete[] encrypted;
        RSA_free(rsa);

        return encryptedEncodedData;
    } catch (const std::exception& e) {
        throw std::runtime_error(e.what());
    }
}

std::string rsaDecrypt(const std::string& encryptedEncodedData, const std::string &privateKeyStr) {
    try {
        // Decode the Base64 encoded data
        std::string encryptedData;
        encryptedData.resize(boost::beast::detail::base64::decoded_size(encryptedEncodedData.size()));

        const auto decoded = boost::beast::detail::base64::decode(&encryptedData[0], encryptedEncodedData.data(), encryptedEncodedData.size());
        encryptedData.resize(decoded.first);

        RSA* rsa = readPrivateKeyFromBuffer(privateKeyStr);
        int rsaLen = RSA_size(rsa);
        unsigned char* decrypted = new unsigned char[rsaLen];

        int result = RSA_private_decrypt(encryptedData.size(), reinterpret_cast<const unsigned char*>(encryptedData.c_str()), decrypted, rsa, RSA_PKCS1_OAEP_PADDING);
        if (result == -1) {
            delete[] decrypted;
            RSA_free(rsa);
            handleOpenSSLErrors();
        }

        std::string decryptedData(reinterpret_cast<char*>(decrypted), result);

        delete[] decrypted;
        RSA_free(rsa);

        return decryptedData;
    } catch (const std::exception& e) {
        throw std::runtime_error(e.what());
    }
}
