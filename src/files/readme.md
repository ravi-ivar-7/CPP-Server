# Downlaod - Upload File

- Download the specified file from the server.
- Upload file to server.

## Features

- Supports various MIME types
- Reads files from the server's filesystem and sends them to the client

- Handles HTTP POST requests with file uploads.
- Extracts filenames from the `Content-Disposition` header.
- Saves uploaded files to the `assets/downloads` directory.


## API Endpoints
- **Downlaod-URL**: `/download-file?fileName=downloadfile.txt`
- **Method**: `GET`
- **Description**: Download a server file, fileName is considered from root folder

- **Upload-URL**: `/upload-file`
- **Method**: `POST`
- **Description**: Upload a file to server


## Code Overview
- `getFileExtension()`
- `downloadFile()`: Handle incoming HTTP request for downloading files.

- `getFileName()`
- `uploadFile()` : Handle Post HTTP request for uploading files.

<!-- 
For file-system link against the Boost Filesystem library when compiling using g++
g++ -o files files.cpp -I/usr/include -lboost_system -lboost_filesystem 
 -->
### Dependencies
- [Boost](https://www.boost.org/): Boost libraries for handling networking and other utilities.