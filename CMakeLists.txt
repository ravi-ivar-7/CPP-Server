# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(FileServer)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Find Boost libraries
find_package(Boost 1.70 REQUIRED COMPONENTS system filesystem)

# Include directories
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/routes)
include_directories(${CMAKE_SOURCE_DIR}/controllers)

# Add executable
add_executable(FileServer server.cpp routes/routes.cpp controllers/controller.cpp)

# Link Boost libraries
target_link_libraries(FileServer PRIVATE ${Boost_LIBRARIES})

