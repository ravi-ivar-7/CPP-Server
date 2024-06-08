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


#include "../includes/homePage.hpp"
#include <boost/beast.hpp>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

void homePage(tcp::socket &&socket, http::request<http::string_body> &&req) {
    std::string htmlContent = R"(
        <!DOCTYPE html>
        <html>
        <head>
            <title>Home Page</title>
            <style>
                body { font-family: Arial, sans-serif; margin: 40px; }
                h1 { color: #333; }
                p { font-size: 18px; }
            </style>
        </head>
        <body>
            <h1>Welcome to the Home Page</h1>
            <p>This is a simple homepage served by a C++ HTTP server using Boost.Beast.</p>
        </body>
        </html>
    )";

    http::response<http::string_body> res{http::status::ok, req.version()};
    res.set(http::field::content_type, "text/html");
    res.body() = htmlContent;
    res.prepare_payload();
    http::write(socket, res);
}
