# Specify the compiler and compilation flags
CC = g++
CFLAGS = -std=c++17

# Specify the target executable name
TARGET = server

# Specify the source files
SRCS = server.cpp routes/routes.cpp controllers/handleFiles.cpp controllers/common.cpp

# Specify the object files
OBJS = $(SRCS:.cpp=.o)

# Specify the Boost library directories
BOOST_INCLUDE_DIRS = /usr/include
BOOST_LIB_DIRS = /usr/lib

# Specify the Boost libraries to link against
BOOST_LIBS = -lboost_system -lboost_filesystem

# Specify the directories for header files
INC_DIRS = -I$(BOOST_INCLUDE_DIRS) -I./routes -I./controllers

# Specify the directory for libraries
LIB_DIRS = -L$(BOOST_LIB_DIRS)

# Build target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INC_DIRS) $(LIB_DIRS) $(OBJS) $(BOOST_LIBS) -o $(TARGET)

# Compile source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
