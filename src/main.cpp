#include <iostream>

#include "CKeyFactory.hpp"

int main() {

    BitHack::CKeyFactory factory;

    std::cout << factory.createKey() << std::endl;
    auto i = 0;
    for (i = 0; i < 10; ++i)
        std::cout << factory.nextKey() << std::endl;

    std::cerr << "Created keys: " << i << std::endl;

    return (0);
}
