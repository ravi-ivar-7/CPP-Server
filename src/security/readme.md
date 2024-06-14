# Security: Encryption - Decryption

## Overview
Functionalities for encryption and decryption using OpenSSL with RSA encryption algorithm. It also includes JSON parsing capabilities using nlohmann/json library. The module is designed to handle data sent via HTTP requests securely.

## Features
- **Encryption and Decryption**: Uses RSA encryption algorithm from OpenSSL for securely encrypting and decrypting data.

## API Endpoints
- **URL**: `/encrypt-data`
- **Method**: `POST`
- **Description**: Encrypts data

- **URL**: `/decrypt-data`
- **Method**: `POST`
- **Description**: decrypts given data

## Code Overview

### Functions
- `readPublicKeyFromBuffer()`: Reads RSA public key from a buffer.
- `readPrivateKeyFromBuffer()`: Reads RSA private key from a buffer.
- `rsaEncrypt(const std::string& data, const std::string& publicKeyStr)`: Encrypts data using RSA public key.
- `rsaDecrypt(const std::string& encryptedData, const std::string& privateKeyStr)`: Decrypts encrypted data using RSA private key.
- `encryptData(tcp::socket&& socket, http::request<http::string_body>&& req)`: Encrypts data received via HTTP request and sends encrypted response.
- `decryptData(tcp::socket&& socket, http::request<http::string_body>&& req)`: Decrypts data received via HTTP request and sends decrypted response.

### Dependencies
- [Boost](https://www.boost.org/): Boost libraries for handling networking and other utilities.
- [OpenSSL](https://www.openssl.org/): OpenSSL for RSA encryption and decryption.
- [nlohmann/json](https://github.com/nlohmann/json): nlohmann/json for JSON parsing and serialization.
- [Boost.Beast](https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/index.html): Boost.Beast for handling HTTP requests and responses.
