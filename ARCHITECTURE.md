# System Architecture - C++ Server

This document provides a comprehensive overview of the C++ server architecture, design patterns, and technical implementation details.

## Overview

The server is built using modern C++17 with Boost.Asio for asynchronous I/O operations and Boost.Beast for HTTP/WebSocket protocol handling. It follows a modular, event-driven architecture designed for high performance and scalability.

## Project Structure

```
CPP-Server/
├── API_USAGE.md              # Endpoint usage guide
├── ARCHITECTURE.md           # System architecture documentation
├── Dockerfile               # Container configuration
├── README.md                # Project overview
├── START.md                 # Quick start guide
├── makefile                 # Build configuration
├── server                   # Compiled executable
│
├── assets/
│   └── logs/
│       └── log.csv          # Server logs
│
├── bcrypt/                  # Password hashing library
│   ├── build/
│   │   └── libbcrypt.a      # Compiled library
│   ├── include/
│   │   └── bcrypt.h
│   └── src/
│       ├── bcrypt.cpp
│       └── blowfish.cpp
│
├── config/
│   ├── privateKey.pem       # RSA private key
│   ├── privateKey.example.pem
│   ├── publicKey.pem        # RSA public key
│   └── server.conf          # Server configuration file
│
├── reports/
│   ├── errors.txt
│   ├── k6_http_loadtest_18_06_24.html
│   └── readme.md
│
├── src/                     # Source code
│   ├── main.cpp             # Entry point with configuration and logging
│   │
│   ├── auth/                # Authentication module
│   │   ├── authenticate.cpp
│   │   ├── authenticate.hpp
│   │   └── readme.md
│   │
│   ├── client/              # Client rendering
│   │   ├── render_html.cpp
│   │   ├── render_html.hpp
│   │   └── readme.md
│   │
│   ├── databases/           # Database integration
│   │   ├── sqlite.cpp
│   │   ├── sqlite.hpp
│   │   └── readme.md
│   │
│   ├── files/               # File operations
│   │   ├── download.cpp
│   │   ├── download.hpp
│   │   ├── upload.cpp
│   │   ├── upload.hpp
│   │   ├── utils.cpp
│   │   ├── utils.hpp
│   │   └── readme.md
│   │
│   ├── health/              # Health monitoring
│   │   ├── health_check.cpp
│   │   └── health_check.hpp
│   │
│   ├── requests/            # HTTP proxy
│   │   ├── get_request.cpp
│   │   ├── post_request.cpp
│   │   ├── utils.cpp
│   │   ├── utils.hpp
│   │   └── readme.md
│   │
│   ├── routes/              # Request routing
│   │   ├── http_route.cpp
│   │   ├── routes.hpp
│   │   ├── ws_route.cpp
│   │   └── readme.md
│   │
│   ├── security/            # Encryption/decryption
│   │   ├── encryption_decryption.cpp
│   │   ├── encryption_decryption.hpp
│   │   ├── utils.cpp
│   │   ├── utils.hpp
│   │   └── readme.md
│   │
│   ├── servers/             # Core server
│   │   ├── http_server.cpp
│   │   ├── http_server.hpp
│   │   └── readme.md
│   │
│   ├── stream/              # Text streaming
│   │   ├── stream_text.cpp
│   │   ├── stream_text.hpp
│   │   └── readme.md
│   │
│   ├── system/              # System utilities
│   │   ├── log.cpp
│   │   ├── log.hpp
│   │   ├── sys_server_info.cpp
│   │   ├── sys_server_info.hpp
│   │   ├── utils.cpp
│   │   ├── utils.hpp
│   │   └── readme.md
│   │
│   ├── utils/               # Core utilities
│   │   ├── config.cpp
│   │   ├── config.hpp
│   │   ├── logger.cpp
│   │   └── logger.hpp
│   │
│   └── websocket/           # WebSocket handling
│       ├── utils.cpp
│       ├── utils.hpp
│       └── readme.md
│
├── templates/               # HTML templates
│   ├── home.html           # Main interface
│   └── stream_text.html    # WebSocket client
│
└── tests/                   # Testing files
    ├── comparision/
    │   ├── fastapi_server.py
    │   ├── nodejs_server.js
    │   ├── package.json
    │   └── package-lock.json
    ├── k6/
    │   ├── cppWsTest-2024-06-18.html
    │   ├── k6_http.js
    │   └── k6_ws.js
    ├── locust/
    │   └── locustfile.py
    ├── instructions.md
    ├── readme.md
    └── rest.http            # API test requests
```

## Core Architecture

### High-Level System Design

```
┌─────────────────────────────────────────────────────────────┐
│                    Client Requests                          │
│              (HTTP/WebSocket/Browser)                       │
└─────────────────────┬───────────────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────────────┐
│                 HttpServer                                  │
│  ┌─────────────────────────────────────────────────────────┐│
│  │              HttpAcceptor                               ││
│  │  • Accepts incoming connections                         ││
│  │  • Creates socket per connection                        ││
│  │  • Routes to HTTP/WebSocket handlers                    ││
│  └─────────────────────────────────────────────────────────┘│
└─────────────────────┬───────────────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────────────┐
│                   Routes                                    │
│  ┌─────────────────┐    ┌─────────────────────────────────┐ │
│  │   HTTP Routes   │    │      WebSocket Routes           │ │
│  │                 │    │                                 │ │
│  └─────────────────┘    └─────────────────────────────────┘ │
└─────────────────────┬───────────────────────────────────────┘
                      │
┌─────────────────────────────────────────────────────────────┐
│                Service Modules                              │
│ ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐ │
│ │  Auth   │ │  Files  │ │Security │ │ System  │ │ Client  │ │
│ └─────────┘ └─────────┘ └─────────┘ └─────────┘ └─────────┘ │
│ ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐ │
│ │Requests │ │ Stream  │ │Database │ │WebSocket│ │ Health  │ │
│ └─────────┘ └─────────┘ └─────────┘ └─────────┘ └─────────┘ │
│ ┌─────────┐ ┌─────────┐                                     │
│ │ Logger  │ │ Config  │                                     │
│ └─────────┘ └─────────┘                                     │
└─────────────────────────────────────────────────────────────┘
```

## Core Components

### 1. HttpServer Class
**Location:** `src/servers/http_server.cpp`

**Responsibilities:**
- Server lifecycle management (start/stop/run)
- Thread pool management for concurrent request handling
- I/O context coordination
- Exception handling and error recovery

**Key Features:**
- **Thread Pool:** Configurable thread pool size (default: 2x CPU cores)
- **Asynchronous I/O:** Non-blocking operations using Boost.Asio
- **Resource Management:** RAII pattern with smart pointers
- **Graceful Shutdown:** Proper cleanup of resources and threads

### 2. HttpAcceptor Class
**Location:** `src/servers/http_server.cpp`

**Responsibilities:**
- Accept incoming TCP connections
- Create dedicated socket per connection
- Route requests to appropriate handlers
- Protocol detection (HTTP vs WebSocket)

**Key Features:**
- **Async Accept:** Non-blocking connection acceptance
- **Socket Reuse:** Address reuse option for development
- **Protocol Routing:** Automatic HTTP/WebSocket detection
- **Connection Management:** Per-connection resource allocation

### 3. Routing System
**Location:** `src/routes/`

**HTTP Routes (`http_route.cpp`):**
```cpp
void Routes::httpRoute(socket, request, buffer)
```

**Supported Endpoints:**
- `GET /` or `/home` → Home page rendering
- `GET /health` → Health check and system monitoring
- `GET /sys-server-info` → System information
- `GET /render-html` → Template rendering
- `GET /download-file` → File download
- `POST /upload-file` → File upload
- `GET /authenticate` → User authentication
- `POST /encrypt-data` → RSA encryption
- `POST /decrypt-data` → RSA decryption
- `GET /get-request` → HTTP proxy (GET)
- `POST /post-request` → HTTP proxy (POST)

**WebSocket Routes (`ws_route.cpp`):**
- Real-time text streaming
- User authentication via headers
- Multi-user communication

## Service Modules

### Authentication Module
**Location:** `src/auth/`

**Features:**
- User credential validation
- Session management
- Security token handling

### File Management Module
**Location:** `src/files/`

**Components:**
- **Upload Handler:** Multipart form data processing
- **Download Handler:** File serving with proper headers
- **Utils:** File system operations and validation

**Security Features:**
- Path traversal protection
- File type validation
- Size limitations

### Security Module
**Location:** `src/security/`

**Encryption Features:**
- **RSA Encryption/Decryption:** 256-bit key support
- **Key Management:** Public/private key handling
- **Data Protection:** Secure data transmission

**Implementation:**
- OpenSSL integration
- PKCS#1 OAEP padding
- Base64 encoding for transport

### System Module
**Location:** `src/system/`

**Components:**
- **System Info:** Hardware and OS information
- **Logging:** Request/error logging to CSV
- **Utils:** System utilities and helpers

### Client Module
**Location:** `src/client/`

**Features:**
- **HTML Rendering:** Template processing
- **Static Content:** CSS/JS serving
- **Dynamic Content:** Parameter-based rendering

### Request Proxy Module
**Location:** `src/requests/`

**Features:**
- **HTTP Proxy:** External API requests
- **SSL Support:** HTTPS connections
- **JSON Processing:** Request/response handling
- **Error Handling:** Network error management

### WebSocket Module
**Location:** `src/websocket/`

**Features:**
- **Real-time Communication:** Bidirectional messaging
- **User Management:** Client identification
- **Authentication:** Token-based access
- **Broadcasting:** Multi-client messaging

### Streaming Module
**Location:** `src/stream/`

**Features:**
- **Text Streaming:** Real-time text communication
- **User Sessions:** Individual user tracking
- **Message Broadcasting:** Group communication

### Health Monitoring Module
**Location:** `src/health/`

**Features:**
- **System Metrics:** CPU, memory, disk usage monitoring
- **Server Status:** Uptime, configuration, dependencies
- **JSON API:** RESTful health endpoint
- **Real-time Data:** Current system state

### Configuration Module
**Location:** `src/utils/config.hpp`

**Features:**
- **File-based Config:** Load from configuration files
- **Environment Variables:** Override with env vars
- **Type Safety:** String, integer, boolean getters
- **Hierarchical Loading:** Defaults → file → environment

### Logging Module
**Location:** `src/utils/logger.hpp`

**Features:**
- **Structured Logging:** Multiple log levels (DEBUG, INFO, WARN, ERROR, FATAL)
- **Multiple Outputs:** Console and file logging
- **Thread Safety:** Mutex-protected operations
- **Contextual Info:** File and line number tracking

## Design Patterns

### 1. Asynchronous Event-Driven Architecture
- **Non-blocking I/O:** All operations use async callbacks
- **Event Loop:** Single-threaded event processing per thread
- **Callback Chains:** Composed async operations

### 2. RAII (Resource Acquisition Is Initialization)
- **Smart Pointers:** Automatic memory management
- **Socket Management:** Automatic cleanup on scope exit
- **Thread Safety:** Mutex-protected shared resources

### 3. Modular Design
- **Separation of Concerns:** Each module handles specific functionality
- **Loose Coupling:** Modules communicate through well-defined interfaces
- **High Cohesion:** Related functionality grouped together

### 4. Factory Pattern
- **Socket Creation:** Dynamic socket allocation per connection
- **Request Handling:** Handler selection based on request type

## Performance Characteristics

### Concurrency Model
- **Thread Pool:** Configurable worker threads
- **Async I/O:** Non-blocking operations
- **Connection Per Thread:** Efficient resource utilization

### Memory Management
- **Smart Pointers:** Automatic memory cleanup
- **Buffer Reuse:** Efficient buffer management
- **Stack Allocation:** Minimal heap usage where possible

### Network Performance
- **Keep-Alive:** Connection reuse support
- **Chunked Transfer:** Large file handling
- **Compression:** Optional response compression

## Security Architecture

### Network Security
- **SSL/TLS Support:** Encrypted connections
- **Input Validation:** Parameter sanitization
- **Path Protection:** Directory traversal prevention

### Data Security
- **RSA Encryption:** Strong cryptographic protection
- **Key Management:** Secure key storage
- **Authentication:** User verification

### Application Security
- **Error Handling:** Secure error responses
- **Logging:** Security event tracking
- **Resource Limits:** DoS protection

## Configuration

### Build Configuration
- **C++17 Standard:** Modern C++ features
- **Compiler Flags:** `-std=c++17 -Wall -Wextra`
- **Dependencies:** Boost, OpenSSL, SQLite3, bcrypt

### Runtime Configuration
- **Port:** Default 8080 (configurable)
- **Thread Pool:** 2x CPU cores (configurable)
- **Buffer Sizes:** Optimized for typical web requests

## Error Handling Strategy

### Exception Hierarchy
1. **Boost System Errors:** Network and I/O errors
2. **Logic Errors:** Programming logic issues
3. **Runtime Errors:** Runtime condition failures
4. **Standard Exceptions:** General C++ exceptions
5. **Unknown Errors:** Catch-all handler

### Error Recovery
- **Graceful Degradation:** Continue serving other requests
- **Resource Cleanup:** Automatic resource deallocation
- **Error Logging:** Comprehensive error tracking

## Scalability Considerations

### Horizontal Scaling
- **Stateless Design:** No server-side session storage
- **Load Balancer Ready:** Multiple instance support
- **Database Separation:** External data storage

### Vertical Scaling
- **Thread Pool Tuning:** Adjustable worker threads
- **Memory Optimization:** Efficient resource usage
- **CPU Utilization:** Multi-core processing

## Development Guidelines

### Code Organization
- **Header Files:** Interface definitions in `.hpp`
- **Implementation:** Logic in `.cpp` files
- **Namespaces:** Boost aliases for clarity

### Best Practices
- **RAII:** Automatic resource management
- **Const Correctness:** Immutable data where possible
- **Exception Safety:** Strong exception guarantees

### Testing Strategy
- **Unit Tests:** Individual component testing
- **Integration Tests:** End-to-end functionality
- **Load Tests:** Performance validation
- **Security Tests:** Vulnerability assessment

## Future Enhancements

### Planned Features
- **Database Integration:** Full ORM support
- **Caching Layer:** Redis/Memcached integration
- **Monitoring:** Metrics and health checks
- **Configuration Management:** External config files

### Performance Improvements
- **HTTP/2 Support:** Modern protocol features
- **Connection Pooling:** Database connection reuse
- **Response Caching:** Static content optimization
- **Compression:** Automatic response compression

## Dependencies

### Core Libraries
- **Boost.Asio:** Asynchronous I/O operations
- **Boost.Beast:** HTTP/WebSocket protocol handling
- **Boost.Filesystem:** File system operations
- **OpenSSL:** Cryptographic operations
- **SQLite3:** Database operations
- **bcrypt:** Password hashing

### Build Tools
- **CMake:** Build system (optional)
- **Make:** Primary build tool
- **GCC/Clang:** C++17 compatible compiler