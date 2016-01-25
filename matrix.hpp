#ifndef matrix_hpp
#define matrix_hpp

#include <algorithm>
#include <complex>

#include "common.hpp"

// fwd declaration: in order to use enable_if
template<typename, typename = void> struct matrix;

// use matrices only for types that are arithmetic (i.e number types)
template<typename T>
struct matrix<T,
        typename std::enable_if<std::is_arithmetic<T>::value || std::is_same<T, std::complex<T>>::value>::type>
{
    typedef unsigned long size_type;

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

    matrix(size_type N, size_type M, container<T> vector)
    : vec(N * M),
      rows(N),
      columns(M)
    {
        if (vector.size() == N * M) {
            std::copy(vector.begin(), vector.end(), vec.begin());
        }
    }

    matrix& operator=(const matrix& other)
    {
        vec = other.vec;
        rows = other.rows;
        columns = other.columns;

        return *this;
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
    container<T> data() const
    {
        return vec;
    }

    /*
     * Extract the columns from this matrix into a
     * vector of vectors so that they can be used in other
     * routines (such as orthogonalization).
     */
    container<container<T>> columnCollection() const
    {
        container<container<T>> result;

        for (size_type i = 0; i < columns; ++i)
        {
            result.push_back(getColumn(i));
        }

        return result;
    }

    /*
     * Extract the rows from this matrix into a vector of
     * vectors so that they can be used in other routines
     * (such as orthogonalization).
     */
    container<container<T>> rowCollection() const
    {
        container<container<T>> result;

        for (size_type i = 0; i < rows; ++i)
        {
            result.push_back(getRow(i));
        }

        return result;
    }
private:
    container<T> vec;
    size_type rows;
    size_type columns;

    container<T> getRow(size_type rowIndex) const
    {
        container<T> result;

        for (size_type i = 0; i < columns; ++i)
        {
            result.push_back(this->operator()(rowIndex, i));
        }

        return result;
    }

    container<T> getColumn(size_type columnIndex) const
    {
        container<T> result;

        for (size_type i = 0; i < rows; ++i)
        {
            result.push_back(this->operator()(i, columnIndex));
        }

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
