```
project/
├── CMakeLists.txt
├── server.cpp
├── routes/
│   ├── routes.cpp
│   └── routes.hpp
├── controllers/
│   ├── controller.cpp
│   └── controller.hpp
└── uploads/
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
./FileServer
```