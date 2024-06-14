```
CPP-Server/
├── CMakeLists.txt
├── makefile
├── .gitignore
├── readme.md
|
|── assets/
│   ├── downloads
|   ├── logs/log.csv
|   ├── js
│   └── css
|
├── config/
│   ├── .env
|   ├── publicKey.pem
│   └── privateKey.pem
|
├── docs/
│   ├── 
│   └── 
|
├── src/
|   ├── auth
|   |       ├── authenticate.cpp
|   |       ├── authenticate.hpp
│   |       └── readme.md
|   ├── client
|   |       ├── render_html.cpp
|   |       ├── render_html.hpp
│   |       └── readme.md
|   ├── files
|   |       ├── download.cpp
|   |       ├── download.hpp
|   |       ├── upload.cpp
|   |       ├── upload.hpp
|   |       ├── utils.cpp
|   |       ├── utils.hpp
│   |       └── readme.md
|   ├── requests
|   |       ├── HttpServic.cpp
|   |       ├── utils.cpp
|   |       ├── utils.hpp
│   |       └── readme.md
|   ├── security
|   |       ├── encryption_decrypton.cpp
|   |       ├── encryption_decrypton.hpp
|   |       ├── utils.cpp
|   |       ├── utils.hpp
│   |       └── readme.md
|   ├── servers
|   |       ├── http_server.cpp
|   |       ├── http_server.hpp
│   |       └── readme.md
|   ├── streaming
|   |       ├── text.cpp
|   |       ├── text.cpp
│   |       └── readme.md
|   ├── system
|   |       ├── log.cpp
|   |       ├── log.hpp
|   |       ├── sys_server_info.cpp
|   |       ├── sys_server_info.hpp
|   |       ├── utils.cpp
|   |       ├── utils.hpp
│   |       └── readme.md
|   ├── utils
|   |       ├── .cpp
|   |       ├── .hpp
│   |       └── readme.md
|
├── templates/
│   |
│   ├── home.html
│   └── client.html
|
├── tests/
│   |
│   ├── locustfile.py
|   ├── rest.http
│   └── api.py

```
## To compile, build, and run the server in WSL:
### Using `makefile` 
```
Go to root directory
Run the make command to compile the server
Run server with compilled file $ ./server
```

### Using `CMake`
```
Create a build directory (recommended for out-of-source builds)
Run cmake to generate the build files
Run make to build the server
Run the server executable from the build directory
```
```
mkdir build
cd build
cmake ..
make
./server
```







## Installation
### Boost

```bash
Download and install it from the [Boost website](https://www.boost.org/).
OR
sudo apt-get update
sudo apt-get install -y libboost-all-dev
```

### Nlohmann/json
```
sudo apt-get install -y nlohmann-json3-dev
```

### OpenSSL
```bash
# On Debian/Ubuntu
sudo apt-get install libssl-dev

# On CentOS/RHEL
sudo yum install openssl-devel
```




## Acknowledgments
- https://www.boost.org/
- https://github.com/nlohmann/json
- https://www.openssl.org/