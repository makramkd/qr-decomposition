//
// Created by Makram Kamaleddine on 1/21/16.
//

#ifndef QR_DECOMPOSITION_OPS_HPP
#define QR_DECOMPOSITION_OPS_HPP

#include <assert.h>

#include "nvector.hpp"

/*
 * An almost general inner product template function: if the
 * nvector has complex numbers inside it this can be the standard
 * inner product used if we're working with complex numbers.
 */
template<typename T, typename BinaryFunc>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
inner_product(const nvector<T>& v1, const nvector<T>& v2, BinaryFunc func)
{
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Vectors must be of the same size");
    }

    // naive algorithm for an inner product
    T result = static_cast<T>(0);
    for (auto i = 0; i < v1.size(); ++i)
    {
        result += func(v1[i], v2[i]);
    }

    return result;
}

/**
 *
 * Project the vector a onto the line
 * spanned by the vector e.
 * */
template<typename T, typename BinaryFunc>
nvector<typename std::enable_if<std::is_floating_point<T>::value, T>::type>
proj(const nvector<T>& e, const nvector<T>& a, BinaryFunc func)
{
    return (inner_product(e, a, func) / inner_product(e, e, func)) * e;
}

/*
 * Transpose the given matrix about the main diagonal
 * (i.e a standard transpose).
 */
template<typename T>
matrix<T> transpose(const matrix<T>& mat)
{
    using size_type = typename matrix<T>::size_type;

    matrix<T> result(mat.colCount(), mat.rowCount());
    for (size_type i = 0; i < result.rowCount(); ++i)
    {
        for (size_type j = 0; j < result.colCount(); ++j)
        {
            result(i, j) = mat(j, i);
        }
    }

    return result;
}

/**
 * Given a set of vectors that constitute the columns of a
 * matrix, construct the columns of the matrix.
 */
template<typename T>
matrix<T> construct_from_column_vectors(const container<container<T>>& basis)
{

#ifdef DEBUG
    // check if all the vectors are the same size
    auto size = basis.front().size();
    for (auto i = basis.begin(); i != basis.end(); ++i)
    {
        assert(size == i->size());
    }
#endif
    using size_type = typename matrix<T>::size_type;

    matrix<T> result(basis.front().size(), basis.size());

    for (size_type j = 0; j < basis.size(); ++j) {
        for (size_type i = 0; i < basis[j].size(); ++i) {
            result(i, j) = (basis[j])[i];
        }
    }

    return result;
}

template<typename T>
matrix<T> construct_from_row_vectors(const container<container<T>>& basis)
{

#ifdef DEBUG
    // check if all the vectors are the same size
    auto size = basis.front().size();
    for (auto i = basis.begin(); i != basis.end(); ++i)
    {
        assert(size == i->size());
    }
#endif
    using size_type = typename matrix<T>::size_type;

    matrix<T> result(basis.size(), basis.front().size());

    for (size_type i = 0; i < basis.size(); ++i) {
        for (size_type j = 0; j < basis[i].size(); ++j) {
            result(i, j) = (basis[i])[j];
        }
    }
    
    return result;
}

/**
 * Given a basis of vectors return the orthonormal basis
 * associated with it. This is essentially the Gram-Schmidt
 * procedure.
 */
template<typename T>
container<container<T>> orthonormalize(const container<container<T>>& basis)
{

}
#endif //QR_DECOMPOSITION_OPS_HPP
