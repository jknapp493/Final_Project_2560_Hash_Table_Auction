#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <string>

using namespace boost::asio;       
using namespace boost::beast;    

int main() {
    try {
        io_context ioc;

        ip::tcp::resolver resolver(ioc);

        auto const results = resolver.resolve("www.example.com", "80");

        ip::tcp::socket socket(ioc);

        connect(socket, results.begin(), results.end());

        http::request<http::string_body> req(http::verb::get, "/", 11);
        req.set(http::field::host, "www.example.com");
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::write(socket, req);

        flat_buffer buffer;

        http::response<http::dynamic_body> res;

        http::read(socket, buffer, res);

        std::cout << res << std::endl;

        boost::system::error_code ec; 
        socket.shutdown(ip::tcp::socket::shutdown_both, ec);

        if (ec && ec != boost::system::errc::not_connected) {
            throw boost::system::system_error{ec}; 
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
