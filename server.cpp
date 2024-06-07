#include <boost/asio.hpp>
#include "routes/routes.hpp"
#include <iostream>

using namespace boost::asio;
using ip::tcp;

int main() {
    try {
        io_context ioContext;

        tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 8080));

        while (true) {
            tcp::socket socket(ioContext);
            acceptor.accept(socket);

            std::thread(handleSession, std::move(socket)).detach();
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
