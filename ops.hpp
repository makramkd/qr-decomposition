//
// Created by Makram Kamaleddine on 1/21/16.
//

#ifndef QR_DECOMPOSITION_OPS_HPP
#define QR_DECOMPOSITION_OPS_HPP

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

    matrix<T> result(mat.rowCount(), mat.colCount());
    for (size_type i = 0; i < result.rowCount(); ++i)
    {
        for (size_type j = 0; j < result.colCount(); ++j)
        {
            result(i, j) = mat(j, i);
        }
    }
}
#endif //QR_DECOMPOSITION_OPS_HPP
