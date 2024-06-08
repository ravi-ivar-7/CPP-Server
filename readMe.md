```
project/
├── CMakeLists.txt
├── makefile
├── main.cpp
├── rest.http
|
├── config/
│   ├── .env
│   └── key.pem
|
├── routers/
│   ├── router.cpp
│   └── router.hpp
|
├── controllers/
|   ├── utils
|   |       ├── HttpServic.cpp
|   |       ├── HttpAcceptor.cpp
│   |       └── HttpServer.cpp
│   |
│   ├── handleFiles.cpp
│   └── common.cpp
|
├── includes/
|   ├── utils
|   |       ├── HttpServic.hpp
|   |       ├── HttpAcceptor.hpp
│   |       └── HttpServer.hpp
│   |
│   ├── handleFiles.hpp
│   └── common.hpp

```
## To compile, build, and run the server in WSL:
### Using `makefile` 
```
Go to root directory
Run the make command to compile the server
Run server with compilled file like ./server
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