#include <iostream>
#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "routes.hpp"
#include "../system/sys_server_info.hpp"
#include "../files/download.hpp"
#include "../files/upload.hpp"
#include "../client/render_html.hpp"
#include "../auth/authenticate.hpp"
#include "../security/encryption_decryption.hpp"
#include "../requests/get_request.hpp"
#include "../requests/post_request.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

void Routes::httpRoute(std::shared_ptr<tcp::socket> socket, std::shared_ptr<http::request<http::string_body>> req, std::shared_ptr<beast::flat_buffer> /*buffer*/ )
{
    std::string clientIp = socket->remote_endpoint().address().to_string();
    std::cout << "[CLIENT IP : " << clientIp << " ]" << std::endl;
    std::cout << "[REQUEST   : " << req->method_string() << req->target() << " ]" << std::endl;

    if (req->method() == http::verb::get && req->target().starts_with("/home"))
    {
        req->target("render-html?filePath=templates/home.html");
        renderHtml(std::move(*socket), std::move(*req));
    }
        
    else if (req->method() == http::verb::get && req->target().starts_with("/favicon.ico "))
        sysServerInfo(std::move(*socket), std::move(*req));
    else if (req->method() == http::verb::get && req->target().starts_with("/sys-server-info"))
        sysServerInfo(std::move(*socket), std::move(*req));
    else if (req->method() == http::verb::get && req->target().starts_with("/download-file"))
        downloadFile(std::move(*socket), std::move(*req));
    else if (req->method() == http::verb::post && req->target().starts_with("/upload-file"))
        uploadFile(std::move(*socket), std::move(*req));
    else if (req->method() == http::verb::get && req->target().starts_with("/render-html"))
        renderHtml(std::move(*socket), std::move(*req));
    else if (req->method() == http::verb::get && req->target().starts_with("/authenticate"))
        authenticate(std::move(*socket), std::move(*req));
    else if (req->method() == http::verb::post && req->target().starts_with("/encrypt-data"))
        encryptData(std::move(*socket), std::move(*req));
    else if (req->method() == http::verb::post && req->target().starts_with("/decrypt-data"))
        decryptData(std::move(*socket), std::move(*req));
    else if (req->method() == http::verb::get && req->target().starts_with("/get-request"))
        getRequest(std::move(*socket), std::move(*req));
    else if (req->method() == http::verb::post && req->target().starts_with("/post-request"))
        postRequest(std::move(*socket), std::move(*req));
    else
    {
        http::response<http::string_body> res{http::status::not_found, req->version()};
        res.set(http::field::server, "c++");
        res.set(http::field::content_type, "text/plain");
        res.body() = "404 Not Found";
        res.prepare_payload();
        http::write(*socket, res);
    }
}
