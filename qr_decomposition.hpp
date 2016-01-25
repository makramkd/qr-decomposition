//
// Created by Makram Kamaleddine on 1/24/16.
//

#ifndef QR_DECOMPOSITION_QR_DECOMPOSITION_HPP
#define QR_DECOMPOSITION_QR_DECOMPOSITION_HPP

#include "ops.hpp"

template<typename T, typename InnerProd>
std::pair<matrix<T>, matrix<T>> qr_decomposition(const matrix<T>& mat, InnerProd inner_prod)
{
    // alias the size_type
    using size_type = typename matrix<T>::size_type;

    // get the columns of the input matrix
    auto col_collection = mat.columnCollection();

    // transform it into a vector of nvector objects
    container<nvector<T>> basis(col_collection.size());
    std::transform(col_collection.begin(), col_collection.end(), std::back_inserter(basis), [](container<T> ctr) {
        return nvector<T>::from_container(ctr);
    });

    // perform gram-schmidt orthogonalization
    auto orthonormal_basis = orthonormalize(basis, inner_prod);
    // transform the basis back into a matrix
    auto q = construct_from_column_vectors(orthonormal_basis);

    // construct r
    matrix<T> r(q.colCount(), q.rowCount());
    for (size_type i = 0; i < r.rowCount(); ++i) {
        for (size_type j = 0; j < i; ++j) {
            r(i, j) = inner_prod(orthonormal_basis[j], basis[i]);
        }
    }

    // return QR
    return std::make_pair(q, r);
}
#endif //QR_DECOMPOSITION_QR_DECOMPOSITION_HPP
