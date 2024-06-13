# File Download Server

Download the specified file from the server.
 
## Features

- HTTP server using Boost.Beast and Boost.Asio
- Handles GET requests to download files
- Supports various MIME types
- Reads files from the server's filesystem and sends them to the client

## Dependencies

- Boost.Beast
- Boost.Asio
- Boost.Filesystem
- Boost.Algorithm

## Installation

1. **Install Boost Libraries**

   Ensure you have Boost libraries installed on your system. You can download and install them from the [Boost website](https://www.boost.org/).

2. **Clone the Repository**

   ```bash
   git clone https://github.com/yourusername/file-download-server.git
   cd file-download-server
