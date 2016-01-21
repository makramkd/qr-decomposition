#include <iostream>

#include "matrix.hpp"

int main() {
    nvector<double> nvector1(3), nvector2(3);
    for (int i = 0; i < nvector1.size(); ++i) {
        nvector1[i] = i;
    }

    for (int i = 0; i < nvector2.size(); ++i) {
        nvector2[i] = i * i + 1;
    }

    std::cout << "Vector 1:" << std::endl;
    std::cout << nvector1 << std::endl;
    std::cout << "Vector 2:" << std::endl;
    std::cout << nvector2 << std::endl;

    std::cout << "The inner product (standard) : " << inner_product(nvector1, nvector2) << std::endl;
}