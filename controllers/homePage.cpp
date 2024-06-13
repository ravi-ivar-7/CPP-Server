#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "../includes/homePage.hpp"
#include <boost/filesystem.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;
namespace fs = boost::filesystem;

// [REQUEST   : GET/admin/home ]
// Error reading request: end of stream


void homePage(tcp::socket &&socket, http::request<http::string_body> &&req)
{
    // Path to the HTML file
    std::string htmlFilePath = "./templates/homePage.html"; 

    // Read the HTML file content
    std::ifstream htmlFile(htmlFilePath);
    if (!htmlFile.is_open())
    {
        // Handle file open error
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::content_type, "text/plain");
        res.body() = "Unable to open HTML file.";
        res.prepare_payload();
        http::write(socket, res);
        return;
    }

    try
    {
        std::stringstream buffer;
        buffer << htmlFile.rdbuf();
        std::string htmlContent = buffer.str();

        // Create the HTTP response
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::content_type, "text/html");
        res.body() = htmlContent;
        res.prepare_payload();
        http::write(socket, res);
    }
    catch(const std::exception& e)
    {
        // Handle file read error
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::content_type, "text/plain");
        res.body() = "Error reading HTML file.";
        res.prepare_payload();
        http::write(socket, res);
    }

    // Close the HTML file
    htmlFile.close();
}


// #include "homePage.hpp"
// #include <fstream>
// #include <sstream>
// #include <iostream>

// // Function to render a basic HTML page
// std::string renderHtml() {
//     return R"(
//     <!DOCTYPE html>
//     <html lang="en">
//     <head>
//         <meta charset="UTF-8">
//         <meta name="viewport" content="width=device-width, initial-scale=1.0">
//         <title>File Upload</title>
//     </head>
//     <body>
//         <h1>Upload a File</h1>
//         <form action="/" method="post" enctype="multipart/form-data">
//             <input type="file" name="file">
//             <button type="submit">Upload</button>
//         </form>
//     </body>
//     </html>
//     )";
// }

// // Function to create an upload form
// std::string createUploadForm() {
//     return R"(
//     <form action="/upload" method="post" enctype="multipart/form-data">
//         <input type="file" name="file">
//         <input type="submit" value="Upload File">
//     </form>
//     )";
// }

// // Function to handle file upload
// std::string handleFileUpload(const std::string& fileContent, const std::string& fileName) {
//     std::ofstream outFile(fileName, std::ios::binary);
//     outFile.write(fileContent.c_str(), fileContent.size());
//     outFile.close();
//     return "File uploaded successfully";
// }

// // Function to parse multipart form data (simplified for illustration)
// std::vector<std::string> parseMultipartFormData(std::string body, const std::string& boundary) {
//     std::vector<std::string> fileContents;
//     size_t pos = 0;
//     std::string delimiter = "--" + boundary;

//     while ((pos = body.find(delimiter)) != std::string::npos) {
//         std::string part = body.substr(0, pos);
//         body.erase(0, pos + delimiter.length() + 2); // Remove boundary and \r\n
//         size_t headerEnd = part.find("\r\n\r\n");
//         if (headerEnd != std::string::npos) {
//             fileContents.push_back(part.substr(headerEnd + 4));
//         }
//     }

//     return fileContents;
// }
