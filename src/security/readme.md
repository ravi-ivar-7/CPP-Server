# Security & Encryption Module

Provides encryption and decryption services for applications. Uses RSA encryption to keep sensitive data safe from prying eyes, whether it's user information, API keys, or any confidential content.

## What It Does

- **RSA Encryption**: Encrypts sensitive data using industry-standard RSA algorithms
- **Secure Decryption**: Safely decrypts data when needed to access it
- **Key Management**: Handles public and private key operations seamlessly
- **JSON Integration**: Works perfectly with JSON data for modern web applications
- **HTTP Security**: Protects data transmitted over HTTP requests

## How to Use It

### Encrypt Data
- **URL**: `/encrypt-data`
- **Method**: `POST`
- **What it does**: Takes plain text data and encrypts it securely
- **Perfect for**: Protecting passwords, API keys, personal information

### Decrypt Data
- **URL**: `/decrypt-data`
- **Method**: `POST`
- **What it does**: Converts encrypted data back to readable format
- **Perfect for**: Accessing stored credentials, reading protected content

## Key Functions

- **readPublicKeyFromBuffer()** - Loads RSA public key for encryption
- **readPrivateKeyFromBuffer()** - Loads RSA private key for decryption
- **rsaEncrypt()** - Encrypts data using public key
- **rsaDecrypt()** - Decrypts data using private key
- **encryptData()** - Handles HTTP encryption requests
- **decryptData()** - Handles HTTP decryption requests

## Dependencies

- [Boost Libraries](https://www.boost.org/) - For networking and utilities
- [OpenSSL](https://www.openssl.org/) - For RSA encryption algorithms
- [nlohmann/json](https://github.com/nlohmann/json) - For JSON data handling
- [Boost.Beast](https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/index.html) - For HTTP request processing

Ensures sensitive data stays protected with enterprise-level security!
