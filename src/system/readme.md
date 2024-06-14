# System
## System-Server-Information

Provides system information such as CPU details, memory information, system uptime, and the number of CPU cores. It utilizes the Boost Asio and Beast libraries for networking and HTTP handling, and the nlohmann/json library for JSON handling.

## Features

- **CPU Information**: Retrieves and displays CPU information from `/proc/cpuinfo`.
- **Memory Information**: Retrieves and displays memory information from `/proc/meminfo`.
- **System Uptime**: Retrieves and displays system uptime from `/proc/uptime`.
- **CPU Cores**: Retrieves and displays the number of CPU cores using `sysconf`.

## API Endpoints
- **URL**: `/sys-server-info?sysInfo=true`
- **Method**: `GET`
- **Description**: Retrieves the server status and optionally detailed system information.

## Code Overview
- `getCpuInfo():` Reads and returns CPU information from /proc/cpuinfo.
- `getMemoryInfo():` Reads and returns memory information from /proc/meminfo.
- `getUpTime():` Reads and returns system uptime from /proc/uptime.
- `getCoresNum():` Retrieves and returns the number of CPU cores using sysconf.

- `sysServerInfo():` Handles incoming HTTP requests and sends back system information in JSON format.

- `saveLog():` Handles log data and save it to a file. 

## Dependencies
- [Boost](https://www.boost.org/)
- [nlohmann/json](https://github.com/nlohmann/json)
- [Boost.Beast](https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/index.html)
