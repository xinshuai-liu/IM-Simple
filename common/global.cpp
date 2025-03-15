#include "global.h"
#include <iostream>
#include <random>  // C++11随机数库

int genRandom() {

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(1000, 9999);

    int random_number = dis(gen);
    return random_number;
}