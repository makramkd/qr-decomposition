#include <iostream>

#include "nvector.hpp"
#include "ops.hpp"

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

    std::cout << "The inner product (standard) : " << inner_product(nvector1, nvector2, [](double x, double y) {
        return x * y;
    }) << std::endl;

    matrix<double> mat(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});

    auto transpose_mat = transpose(mat);

    std::cout << "Before transpose: mat = \n" << mat << std::endl;
    std::cout << "After transpose: mat^T = \n" << transpose_mat << std::endl;
}