# File Management Module

Handles all file upload and download needs. Let users download resources or upload files with simple and secure operations.

## What It Does

- **File Downloads**: Serves files from server to users
- **File Uploads**: Accepts and safely stores files uploaded by users
- **MIME Type Support**: Automatically detects and handles various file types
- **Smart File Handling**: Extracts filenames and manages file storage automatically
- **Secure Storage**: Saves uploaded files to the `assets/downloads` directory

## How to Use It

### Download Files
- **URL**: `/download-file?fileName=downloadfile.txt`
- **Method**: `GET`
- **What it does**: Downloads any file from server's root directory
- **Perfect for**: Sharing documents, images, or any downloadable content

### Upload Files
- **URL**: `/upload-file`
- **Method**: `POST`
- **What it does**: Accepts file uploads from users
- **Perfect for**: User profile pictures, document submissions, file sharing

## Key Functions

- **getFileExtension()** - Determines the file type for proper handling
- **downloadFile()** - Processes download requests and serves files
- **getFileName()** - Extracts filenames from upload headers
- **uploadFile()** - Handles file upload requests securely

## Dependencies

- [Boost Libraries](https://www.boost.org/) - For networking and file system operations

Takes care of all complex file handling for application logic focus!