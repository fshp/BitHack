#include "openssl/ec.h"
#include "openssl/obj_mac.h"
#include "base58.h"
#include "utils.h"
#include "CKeyFactory.hpp"

#include <iostream>

int main() {

    BitHack::CKeyFactory factory;

    for(auto i = 0; i < 10000; ++i)
        std::cout << factory.createKey();

    return (0);
}
