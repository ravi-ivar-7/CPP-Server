# HTTP Request Proxy Module

Turn server into a powerful HTTP proxy, allowing requests to external APIs and services on behalf of clients. Perfect for integrating with third-party services while keeping API keys secure.

## What It Does

- **Proxy GET Requests**: Fetches data from external APIs and returns it to clients
- **Proxy POST Requests**: Sends data to external services through server
- **Secure Integration**: Keeps API credentials on the server side
- **Flexible Routing**: Works with any HTTP-based API or service

## How to Use It

### Make External POST Requests
- **URL**: `/post-request?host=jsonplaceholder.typicode.com&ep=/posts`
- **Method**: `POST`
- **What it does**: Sends a POST request to the specified external API
- **Perfect for**: Creating records, submitting forms, sending data to third-party services

### Fetch External Data
- **URL**: `/get-request?host=jsonplaceholder.typicode.com&ep=/posts/100`
- **Method**: `GET`
- **What it does**: Retrieves data from external APIs and returns it to client
- **Perfect for**: Fetching user data, getting API responses, integrating external content

## Key Functions

- **postRequest()** - Handles outgoing POST requests to external APIs
- **getRequest()** - Manages GET requests and data retrieval from external services

## Dependencies

- [Boost.Asio](https://www.boost.org/) - For asynchronous networking operations
- [Boost.Beast](https://www.boost.org/) - For HTTP and WebSocket protocol handling

Makes it easy to integrate with any external service while maintaining security and performance!



