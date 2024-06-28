# Specify the compiler and compilation flags
CC = g++
CFLAGS := -std=c++17 -Wall -Wextra

# Specify the target executable name
TARGET = server

# Source files
SRCS := $(wildcard src/*.cpp) \
        $(wildcard src/auth/*.cpp) \
        $(wildcard src/client/*.cpp) \
        $(wildcard src/files/*.cpp) \
        $(wildcard src/requests/*.cpp) \
        $(wildcard src/security/*.cpp) \
        $(wildcard src/servers/*.cpp) \
        $(wildcard src/websocket/*.cpp) \
        $(wildcard src/system/*.cpp) \
        $(wildcard src/stream/*.cpp) \
        $(wildcard src/routes/*.cpp) \
        $(wildcard src/utils/*.cpp)

# Specify the object files
OBJS = $(SRCS:.cpp=.o)

# Specify the Boost library directories
BOOST_INCLUDE_DIRS = /usr/include
BOOST_LIB_DIRS = /usr/lib



# Include directories // header files folder
INC_DIRS := -I./src -I./src/auth -I./src/client -I./src/files -I./src/requests -I./src/security -I./src/servers -I./src/system -I./src/utils -I./src/databases -I./src/websocket -I./src/stream -I./bcrypt/include

# Specify the directory for libraries (including bcrypt)
LIB_DIRS = -L$(BOOST_LIB_DIRS) -L./bcrypt/build

# Libraries to link against (including bcrypt)
LIBS := -lboost_system -lboost_filesystem -lbcrypt -lssl -lcrypto -lsqlite3 -pthread


# Build target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INC_DIRS) $(LIB_DIRS) $(OBJS) $(LIBS) -o $(TARGET)

# Compile source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
