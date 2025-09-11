# C++ HTTP Server with Boost.Asio
A fast C++ HTTP server built with Boost.Asio that handles authentication, file operations, encryption, database connections, and real-time WebSocket messaging.

## Quick Links
- **[START.md](START.md)** - Build and run the server
- **[API_USAGE.md](API_USAGE.md)** - How to use all endpoints with examples
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - Technical design and project structure

**Server runs on:** http://localhost:8080/

## Features

- User authentication and session management
- File upload/download with security checks
- HTTP GET/POST request proxying to external APIs
- RSA encryption and decryption for secure data
- HTML template rendering and static file serving
- SQLite database integration
- Real-time WebSocket messaging




## Available Endpoints

**Web Interface:**
- `GET /` or `/home` - Main web interface
- `GET /render-html?filePath=templates/stream_text.html` - WebSocket chat interface

**API Endpoints:**
- `GET /health` - Health check and system monitoring
- `GET /sys-server-info?sysInfo=true` - System information
- `GET /download-file?filePath=<path>` - Download files
- `POST /upload-file` - Upload files
- `GET /authenticate` - User authentication
- `POST /encrypt-data` - RSA encryption
- `POST /decrypt-data` - RSA decryption
- `GET /get-request?host=<host>&ep=<endpoint>` - HTTP GET proxy
- `POST /post-request?host=<host>&ep=<endpoint>` - HTTP POST proxy

**Usage examples and testing instructions:** [API_USAGE.md](API_USAGE.md)

## Getting Started

1. **Install dependencies:** `sudo apt-get install -y libboost-all-dev nlohmann-json3-dev libssl-dev libsqlite3-dev cmake`
2. **Build bcrypt:** `cd bcrypt && mkdir -p build && cd build && cmake .. && make && cd ../..`
3. **Build server:** `make clean && make`
4. **Run server:** `./server`

**Complete setup guide:** [START.md](START.md)

## Testing the Server

**Browser testing:**
- Home page: http://localhost:8080/
- Health check: http://localhost:8080/health
- System info: http://localhost:8080/sys-server-info?sysInfo=true
- WebSocket chat: http://localhost:8080/render-html?filePath=templates/stream_text.html

**API testing:**
- Use the provided `tests/rest.http` file with REST Client extension
- Or use curl commands from [API_USAGE.md](API_USAGE.md)

## Key Files

- `src/main.cpp` - Server entry point with configuration and logging
- `src/routes/http_route.cpp` - HTTP request routing
- `src/routes/ws_route.cpp` - WebSocket routing
- `src/health/health_check.cpp` - Health monitoring endpoint
- `src/utils/logger.hpp` - Structured logging system
- `src/utils/config.hpp` - Configuration management
- `templates/home.html` - Main web interface
- `templates/stream_text.html` - WebSocket chat client
- `config/server.conf` - Server configuration file
- `config/` - RSA keys for encryption
- `tests/rest.http` - API test requests
- `Dockerfile` - Container deployment
- `DEPLOYMENT.md` - Production deployment guide

## Dependencies

- Boost Libraries (system, filesystem, asio)
- nlohmann/json - JSON parsing
- OpenSSL - Encryption/SSL support
- SQLite3 - Database support
- bcrypt - Password hashing (built from source)




## Performance & Testing

Load test reports are available in `reports/` directory.

## Acknowledgments
- [Boost C++ Libraries](https://www.boost.org/)
- [nlohmann/json](https://github.com/nlohmann/json)
- [OpenSSL](https://www.openssl.org/)
