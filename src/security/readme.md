# CPP-Server

## Overview
This project is a C++ server that uses Boost.Asio for networking, OpenSSL for encryption and decryption, and nlohmann/json for JSON parsing. The server can encrypt and decrypt data sent via HTTP requests.

## Features
- **Encryption and Decryption**: Utilizes RSA for encrypting and decrypting data.
- **JSON Handling**: Uses nlohmann/json for parsing JSON data.
- **Networking**: Implements a server using Boost.Asio for handling HTTP requests.

## Dependencies
- [Boost](https://www.boost.org/)
- [OpenSSL](https://www.openssl.org/)
- [nlohmann/json](https://github.com/nlohmann/json)
- [Boost.Beast](https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/index.html)

## Installation
### Boost
Make sure Boost is installed on your system. You can download and install it from the [Boost website](https://www.boost.org/).

### OpenSSL
Ensure OpenSSL is installed. You can install it via your package manager:
```bash
# On Debian/Ubuntu
sudo apt-get install libssl-dev

# On CentOS/RHEL
sudo yum install openssl-devel
