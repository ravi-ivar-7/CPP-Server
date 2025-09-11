# Quick Start - C++ Server

## One-Command Setup

```bash
# Install all dependencies
sudo apt-get update && sudo apt-get install -y libboost-all-dev nlohmann-json3-dev libssl-dev libsqlite3-dev cmake

# Build bcrypt library
cd bcrypt && mkdir -p build && cd build && cmake .. && make && cd ../..

# Build and run server
make clean && make
./server # or gdb --batch --ex run --ex bt --ex quit --args ./server
```

## 📋 Step-by-Step Instructions

### 1. Install Dependencies
```bash
sudo apt-get update
sudo apt-get install -y libboost-all-dev nlohmann-json3-dev libssl-dev libsqlite3-dev cmake
```

### 2. Build bcrypt Library
```bash
cd bcrypt
mkdir -p build && cd build
cmake ..
make
cd ../..
```

### 3. Build Server
```bash
make clean
make
```

### 4. Run Server
```bash
chmod +x server  # If needed
./server
```

## Success Indicators

When the server starts, you'll see:
```
SERVER STARTED
HTTP PORT: 8080
```

## Access Points

- Home Page: http://localhost:8080/
- Alternative: http://localhost:8080/home
- System Info: http://localhost:8080/sys-server-info

## Troubleshooting

Build Errors:
- Install all dependencies first
- Build bcrypt before main server: `cd bcrypt && mkdir -p build && cd build && cmake .. && make`
- Try clean rebuild: `make clean && make`

Runtime Errors:
- Check if port 8080 is free: `netstat -tulpn | grep 8080`
- Fix permissions: `chmod +x server`
- Check libraries: `ldd server`

Common Issues:
- Missing bcrypt library - build it first
- Permission denied - use `chmod +x server`
- Port already in use - kill existing process or change port