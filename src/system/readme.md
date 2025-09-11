# System Information Module

Gives complete visibility into server health and performance. Like having a dashboard that shows everything about the system - from CPU usage to memory stats and uptime information.

## What It Does

- **CPU Monitoring**: Shows detailed information about server's processor
- **Memory Tracking**: Displays current memory usage and availability
- **Uptime Statistics**: Tells how long server has been running
- **Core Detection**: Identifies the number of CPU cores available
- **System Logging**: Saves important system events to log files
- **Real-Time Data**: Provides up-to-the-minute system information

## How to Use It

### Get System Information
- **URL**: `/sys-server-info?sysInfo=true`
- **Method**: `GET`
- **What it does**: Returns comprehensive system statistics in JSON format
- **Perfect for**: Monitoring dashboards, health checks, performance analysis

## Key Functions

- **getCpuInfo()** - Reads detailed CPU information from system files
- **getMemoryInfo()** - Retrieves current memory usage statistics
- **getUpTime()** - Shows how long the system has been running
- **getCoresNum()** - Counts available CPU cores for performance optimization
- **sysServerInfo()** - Handles system info requests and formats responses
- **saveLog()** - Records important events to log files for troubleshooting

## Dependencies

- [Boost Libraries](https://www.boost.org/) - For networking and system operations
- [nlohmann/json](https://github.com/nlohmann/json) - For JSON data formatting
- [Boost.Beast](https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/index.html) - For HTTP request handling

Helps keep server running smoothly by providing all the insights needed!
