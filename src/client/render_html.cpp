#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include "../files/utils.hpp"
#include "../requests/utils.hpp"
#include "render_html.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

void renderHtml(tcp::socket &&socket, http::request<http::string_body> &&req)
{
    std::string htmlFilePath;
    std::string target = req.target().to_string();
    size_t pos = target.find('?');
    if (pos != std::string::npos)
    {
        std::unordered_map<std::string, std::string> queryParams;
        std::string queryString = target.substr(pos + 1);
        queryParams = getQueryParams(queryString);
        auto it = queryParams.find("filePath");
        if (it != queryParams.end())
            htmlFilePath = it->second;
        else
        {
            std::cerr << "NO FILE PATH PROVIDED" << std::endl;
            throw std::runtime_error("No file path provided");
        }
    }

    try
    {
        std::string htmlContent = readFile(htmlFilePath);
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::content_type, "text/html");
        res.body() = htmlContent;
        res.prepare_payload();
        http::write(socket, res);
    }
    catch (const std::exception &e)
    {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::content_type, "text/plain");
        res.body() = "Error reading HTML file.";
        res.prepare_payload();
        http::write(socket, res);
    }
}
