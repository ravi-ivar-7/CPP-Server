# API Usage Guide

How to use all server endpoints with examples.

## Base URL
```
http://localhost:8080
```

## Endpoints Overview

### 1. Health Check
**GET /health**
- Returns comprehensive server health and system status
- No parameters required
- JSON response with server metrics

**Browser:**
```
http://localhost:8080/health
```

**curl:**
```bash
curl http://localhost:8080/health
```

**Response Example:**
```json
{
  "status": "healthy",
  "timestamp": 1694511600,
  "uptime": "2h 15m 30s",
  "server": {
    "name": "C++ HTTP Server",
    "version": "1.0.0",
    "port": 8080
  },
  "system": {
    "hostname": "server-host",
    "memory_total_kb": 8388608,
    "memory_available_kb": 4194304,
    "disk_total_bytes": 107374182400,
    "disk_free_bytes": 53687091200,
    "disk_usage_percent": 50.0
  },
  "config": {
    "log_level": "INFO",
    "thread_pool_size": 4
  },
  "dependencies": {
    "boost": "ok",
    "openssl": "ok",
    "sqlite": "ok",
    "bcrypt": "ok"
  }
}
```

---

### 2. Home Page
**GET /** or **GET /home**
- Serves the main HTML interface
- No parameters required

**Browser:**
```
http://localhost:8080/
http://localhost:8080/home
```

**curl:**
```bash
curl http://localhost:8080/
```

---

### 2. System Information
**GET /sys-server-info**
- Returns server and system information
- **Parameter:** `sysInfo=true`

**Browser:**
```
http://localhost:8080/sys-server-info?sysInfo=true
```

**curl:**
```bash
curl "http://localhost:8080/sys-server-info?sysInfo=true"
```

---

### 3. Render HTML Templates
**GET /render-html**
- Renders HTML files from templates directory
- **Parameter:** `filePath=<template_path>`

**Examples:**
```bash
# Render home page
curl "http://localhost:8080/render-html?filePath=templates/home.html"

# Render stream text page
curl "http://localhost:8080/render-html?filePath=templates/stream_text.html"
```

**Browser:**
```
http://localhost:8080/render-html?filePath=templates/home.html
```

---

### 4. File Download
**GET /download-file**
- Downloads files from server
- **Parameter:** `filePath=<file_path>`

**Examples:**
```bash
# Download test file
curl "http://localhost:8080/download-file?filePath=tests/rest.http" -o downloaded_file.http

# Download any file (if exists)
curl "http://localhost:8080/download-file?filePath=README.md" -o readme.md
```

**Browser:**
```
http://localhost:8080/download-file?filePath=tests/rest.http
```

---

### 5. File Upload
**POST /upload-file**
- Uploads files to server
- **Content-Type:** `multipart/form-data`
- **Headers:** Content-Disposition with filename

**curl:**
```bash
# Upload a file
curl -X POST http://localhost:8080/upload-file \
  -H "Content-Type: multipart/form-data" \
  -F "file=@/path/to/your/file.txt"

# Upload with specific filename
curl -X POST http://localhost:8080/upload-file \
  -H "Content-Type: multipart/form-data" \
  -H "Content-Disposition: form-data; name=\"file\"; filename=\"myfile.txt\"" \
  --data-binary @/path/to/your/file.txt
```

---

### 6. Authentication
**GET /authenticate**
- User authentication endpoint
- **Content-Type:** `application/json`
- **Body:** JSON with username and password

**curl:**
```bash
curl -X GET http://localhost:8080/authenticate \
  -H "Content-Type: application/json" \
  -d '{
    "username": "user",
    "password": "userpassword"
  }'
```

---

### 7. Data Encryption
**POST /encrypt-data**
- Encrypts data using RSA encryption
- **Content-Type:** `application/json`
- **Body:** JSON with data to encrypt

**curl:**
```bash
curl -X POST http://localhost:8080/encrypt-data \
  -H "Content-Type: application/json" \
  -d '{
    "data1": "hello i am ravi kumar"
  }'
```

Response will contain the encrypted data string.

---

### 8. Data Decryption
**POST /decrypt-data**
- Decrypts RSA encrypted data
- **Content-Type:** `application/json`
- **Body:** JSON with encrypted data

**curl:**
```bash
curl -X POST http://localhost:8080/decrypt-data \
  -H "Content-Type: application/json" \
  -d '{
    "data": "WwNMzm0FZE3PA5mRV3aDzmVxJIXzVxznXwTbboUVFB0ToOIt/e3WDeGXU13liZ8HTXAQ9fwCnPtdo3PmIY6vUYP3Xu01svzMFdjnPac5sEzrOG7sBEnvMH1x22F47TGsL7aHOelj30rzZrUdSR4aYY6c5A3PEhRM/Ut6yBCl15k="
  }'
```

---

### 9. HTTP GET Proxy
**GET /get-request**
- Makes GET requests to external APIs
- **Parameters:** 
  - `host=<target_host>`
  - `ep=<endpoint_path>`

**Examples:**
```bash
# Get a post from JSONPlaceholder
curl "http://localhost:8080/get-request?host=jsonplaceholder.typicode.com&ep=/posts/1"

# Get users list
curl "http://localhost:8080/get-request?host=jsonplaceholder.typicode.com&ep=/users"
```

**Browser:**
```
http://localhost:8080/get-request?host=jsonplaceholder.typicode.com&ep=/posts/100
```

---

### 10. HTTP POST Proxy
**POST /post-request**
- Makes POST requests to external APIs
- **Parameters:** 
  - `host=<target_host>`
  - `ep=<endpoint_path>`
- **Content-Type:** `application/json`
- **Body:** JSON data to send

**curl:**
```bash
curl -X POST "http://localhost:8080/post-request?host=jsonplaceholder.typicode.com&ep=/posts" \
  -H "Content-Type: application/json" \
  -d '{
    "data": "hello world"
  }'
```

---

## Testing with Browser

GET endpoints work directly in browser:

1. Home Page: http://localhost:8080/
2. Health Check: http://localhost:8080/health
3. System Info: http://localhost:8080/sys-server-info?sysInfo=true
4. Download File: http://localhost:8080/download-file?filePath=tests/rest.http
5. External API: http://localhost:8080/get-request?host=jsonplaceholder.typicode.com&ep=/posts/1

For POST endpoints use:
- curl (command line)
- Postman (GUI app)
- Thunder Client (VS Code extension)
- REST Client (VS Code extension)

---

## Using the Provided Test File

The server includes a `tests/rest.http` file with pre-configured requests. If you have the REST Client extension in VS Code:

1. Open `tests/rest.http`
2. Update the port from 7000 to 8080 in the variables section:
   ```
   @Host = localhost
   @Port = 8080
   ```
3. Click "Send Request" above each endpoint

---

## Response Formats

Success:
- HTML pages return rendered content
- File downloads return file with proper headers
- JSON APIs return data from external services
- Encryption returns processed data

Errors:
- 404 Not Found - invalid endpoint or missing file
- 500 Internal Server Error - server processing error
- 400 Bad Request - invalid parameters

---

## WebSocket Endpoints

For WebSocket streaming functionality, see the home page interface at:
```
http://localhost:8080/render-html?filePath=templates/stream_text.html
```

This provides real-time text streaming capabilities with user authentication.
