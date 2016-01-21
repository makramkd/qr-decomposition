#ifndef matrix_hpp
#define matrix_hpp

#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>
#include "nvector.hpp"

// fwd declaration: in order to use enable_if
template<typename, typename = void> struct matrix;

// use matrices only for types that are arithmetic (i.e number types)
template<typename T>
struct matrix<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {

    typedef unsigned int size_type;

    matrix(size_type N, size_type M)
    : vec(N * M),
      rows(N),
      columns(M)
    {

    }

    matrix(size_type N, size_type M, T filler)
    : vec(N * M),
      rows(N),
      columns(M)
    {
        std::fill(vec.begin(), vec.end(), filler);
    }

    matrix(const matrix& other)
    : vec(other.vec),
      rows(other.rows),
      columns(other.columns)
    {

    }

    matrix(size_type N, size_type M, std::initializer_list<T> lst)
    : vec(N * M),
      rows(N),
      columns(M)
    {
        // only copy if they're the same size as the vector
        if (lst.size() == N * M) {
            std::copy(lst.begin(), lst.end(), vec.begin());
        }
    }

    matrix(size_type N, size_type M, std::vector<T> vector)
    : vec(N * M),
      rows(N),
      columns(M)
    {
        if (vector.size() == N * M) {
            std::copy(vector.begin(), vector.end(), vec.begin());
        }
    }

    T& operator()(size_type i, size_type j)
    {
        return vec[i * columns + j];
    }

    const T& operator()(size_type i, size_type j) const
    {
        return vec[i * columns + j];
    }

    size_type rowCount() const
    {
        return rows;
    }

    size_type colCount() const
    {
        return columns;
    }

    // data interface: return internal data
    std::vector<T> data() const
    {
        return vec;
    }

    /*
     * Extract the columns from this matrix into a
     * vector of vectors so that they can be used in other
     * routines (such as orthogonalization).
     */
    std::vector<std::vector<T>> columnCollection() const
    {
        std::vector<std::vector<T>> result;

        return result;
    }

    /*
     * Extract the rows from this matrix into a vector of
     * vectors so that they can be used in other routines
     * (such as orthogonalization).
     */
    std::vector<std::vector<T>> rowCollection() const
    {
        std::vector<std::vector<T>> result;

        return result;
    }
private:
    std::vector<T> vec;
    const size_type rows;
    const size_type columns;

    std::vector<T> getRow(size_type rowIndex) const
    {
        std::vector<T> result;

        return result;
    }

    std::vector<T> getColumn(size_type columnIndex) const
    {
        std::vector<T> result;

        return result;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const matrix<T>& mat)
{
    using size_type = typename matrix<T>::size_type;
    stream << "[";
    for (size_type i = 0; i < mat.rowCount(); ++i)
    {
        for (size_type j = 0; j < mat.colCount(); ++j)
        {
            if (j != mat.colCount() - 1) {
                stream << mat(i, j) << ", ";
            } else {
                stream << mat(i, j);
            }
        }
        if (i != mat.rowCount() - 1) {
            stream << ";" << std::endl;
        }
    }
    stream << "]";
    return stream;
}
#endif /* matrix_hpp */
