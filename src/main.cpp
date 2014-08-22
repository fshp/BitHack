#include <iostream>

#include "CKeyFactory.hpp"

int main() {

    BitHack::CKeyFactory factory;

    std::cout << factory.createKey();
    auto i = 0;
    for (i = 0; i < 100000; ++i)
        std::cout << factory.nextKey();

    std::cerr << "Create key: " << i << std::endl;

    return (0);
}
