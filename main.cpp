#include <iostream>
#include <stdexcept>
#include "./network/networkmanager.h"

int main() {
    try {

        NetworkManager::getInstance().recvData("0.0.0.0", 8080);

    }
    catch (std::exception& exp) {
        std::cout << "exception" << std::endl;
    }

    return 0;
}