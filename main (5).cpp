#include <boost/system/system_error.hpp>
#include <iostream>

int main() {
    try {
        throw boost::system::system_error(boost::system::error_code(), "Boost is working!");
    } catch (const boost::system::system_error &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
