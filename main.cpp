#include <iostream>

#include "nvector.hpp"
#include "ops.hpp"

int main() {
    container<container<float>> basis;
    basis.push_back({1.5, 2.5, 3.5});
    basis.push_back({3.5, 4.5, 5.5});
    basis.push_back({3.5, 8.2, 1.2});

    auto mat = construct_from_column_vectors(basis);

    std::cout << "Matrix: \n" << mat << "\n";
}