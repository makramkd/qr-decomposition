#include <iostream>

#include "nvector.hpp"
#include "ops.hpp"

int main() {
    using std::cout;

    container<nvector<double>> basis;
    basis.push_back(nvector<double>(3, {12, 6, -4}));
    basis.push_back(nvector<double>(3, {-51, 167, 24}));
    basis.push_back(nvector<double>(3, {4, -68, -41}));

    cout << "Basis before orthonormalization: \n";
    for (auto i = basis.begin(); i != basis.end(); ++i) {
        cout << *i << "\n";
        cout << i->norm() << "\n";
    }

    auto std_inner = [](double v1, double v2) {
        return v1 * v2;
    };
    auto orthonormal = orthonormalize(basis, std_inner);

    cout << "Orthonormalized Basis: \n";
    for (auto i = orthonormal.begin(); i != orthonormal.end(); ++i) {
        cout << *i << "\n";
    }

    auto check = check_orthonormality(orthonormal, std_inner);
    cout << "Orthonormal sum: " << check << "\n";
}