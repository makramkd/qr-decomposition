#ifndef matrix_hpp
#define matrix_hpp

#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>

// fwd declaration: in order to use enable_if
template<typename, typename = void> struct matrix;

// use matrices only for types that are arithmetic (i.e number types)
template<typename T>
struct matrix<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {

    matrix(unsigned N, unsigned M)
    : vec(N * M),
      rows(N),
      columns(M)
    {

    }

    matrix(unsigned N, unsigned M, T filler)
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

    matrix(unsigned N, unsigned M, std::initializer_list<T> lst)
    : vec(N * M),
      rows(N),
      columns(M)
    {
        // only copy if they're the same size as the vector
        if (lst.size() == N * M) {
            std::copy(lst.begin(), lst.end(), vec.begin());
        }
    }

    matrix(unsigned N, unsigned M, std::vector<T> vector)
    : vec(N * M),
      rows(N),
      columns(M)
    {
        if (vector.size() == N * M) {
            std::copy(vector.begin(), vector.end(), vec.begin());
        }
    }

    T& operator()(unsigned int i, unsigned int j)
    {
        return vec[i * columns + j];
    }

    const T& operator()(unsigned int i, unsigned int j) const
    {
        return vec[i * columns + j];
    }

    unsigned int rowCount() const
    {
        return rows;
    }

    unsigned int colCount() const
    {
        return columns;
    }

    // data interface: return internal data
    std::vector<T> data() const
    {
        return vec;
    }

    T infnorm() const
    {
        // store rowsums in v
        std::vector<T> v(rows);
        for (auto i = 0; i < rows; ++i)
        {
            std::vector<T> temp(columns);
            for (auto j = 0; j < columns; ++j)
            {
                temp[j] = this->operator()(i, j);
            }
            // transform everything into positive
            std::for_each(temp.begin(), temp.end(), [](T value) -> T{
                return std::abs(value);
            });
            // accumulate
            v[i] = std::accumulate(temp.begin(), temp.end(), 0.0);
        }
        return *(std::max_element(v.begin(), v.end()));
    }

private:
    std::vector<T> vec;
    const unsigned int rows;
    const unsigned int columns;
};

// forward declaration
template<typename, typename = void> struct nvector;

// partial specialization of matrix in order to add some familiar operator[] syntax
// rather than using operator() for both matrices and vectors
template<typename T>
struct nvector<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {

    nvector(unsigned N)
    : vec(N, 1)
    {

    }

    nvector(unsigned N, T filler)
    : vec(N, 1, filler)
    {

    }

    nvector(const nvector& other)
    : vec(other.vec)
    {

    }

    nvector(unsigned N, std::initializer_list<T> lst)
    : vec(N, 1, lst)
    {

    }

    T& operator[](unsigned int i)
    {
        return vec(i, 0);
    }

    const T& operator[](unsigned int i) const
    {
        return vec(i, 0);
    }

    unsigned int size() const
    {
        return vec.rowCount();
    }

    typename std::vector<T>::const_iterator begin() const
    {
        return vec.data().cbegin();
    }

    typename std::vector<T>::const_iterator end() const
    {
        return vec.data().cend();
    }

    T inf_norm() const
    {
        std::vector<T> data(vec.data());
        std::for_each(data.begin(), data.end(), [](T value) -> T {
            return std::abs(value);
        });
        return *(std::max_element(data.begin(), data.end()));
    }
private:
    matrix<T> vec;
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const nvector<T>& nvec)
{
    stream << "[";
    for (auto i = 0; i < nvec.size(); ++i)
    {
        if (i != nvec.size() - 1) {
            stream << nvec[i] << "; ";
        } else {
            stream << nvec[i];
        }
    }
    stream << "]";
    return stream;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const matrix<T>& mat)
{
    stream << "[";
    for (auto i = 0; i < mat.rowCount(); ++i)
    {
        for (auto j = 0; j < mat.colCount(); ++j)
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

// overload operator* for matrix-vector multiplication
template<typename T>
nvector<T> operator*(const matrix<T>& M, const nvector<T>& v)
{
    // dimension check
    if (M.colCount() != v.size()) {
        throw std::invalid_argument("Matrix and vector dimensions are not compatible");
    }

    nvector<T> result(v.size());
    for (auto i = 0; i < M.rowCount(); ++i)
    {
        T sum = 0;
        // do an inner product
        for (auto j = 0; j < M.colCount(); ++j)
        {
            sum += M(i, j) * v[j];
        }
        result[i] = sum;
    }

    return result;
}

// scalar-vector multiplication
template<typename T>
nvector<T> operator*(const T& scalar, const nvector<T>& v)
{
    nvector<T> result(v.size());
    for (auto i = 0; i < result.size(); ++i)
    {
        result[i] = v[i] * scalar;
    }

    return result;
}

template<typename T>
nvector<T> operator-(const nvector<T>& v1, const nvector<T>& v2)
{
    // dim check
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Vector dimensions are not equal");
    }

    nvector<T> result(v1.size());
    for (auto i = 0; i < v1.size(); ++i)
    {
        result[i] = v1[i] - v2[i];
    }

    return result;
}

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
inner_product(const nvector<T>& v1, const nvector<T>& v2)
{
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Vectors must be of the same size");
    }

    // naive algorithm for an inner product
    T result = static_cast<T>(0);
    for (auto i = 0; i < v1.size(); ++i)
    {
        result += v1[i] * v2[i];
    }

    return result;
}

/**
 *
 * Project the vector a onto the line
 * spanned by the vector e.
 * */
template<typename T>
nvector<typename std::enable_if<std::is_floating_point<T>::value, T>::type>
proj(const nvector<T>& e, const nvector<T>& a)
{
    return (inner_product(e, a) / inner_product(e, e)) * e;
}

#endif /* matrix_hpp */
