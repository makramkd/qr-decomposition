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
T relative_residual(const matrix<T>& m, const nvector<T>& rhs, const nvector<T>& xbar)
{
    auto rbar = rhs - (m * xbar);
    return rbar.inf_norm() / rhs.inf_norm();
}

template<typename T>
T relative_error(const nvector<T>& xbar, const nvector<T>& xstar)
{
    auto numerator = xbar - xstar;
    return numerator.inf_norm() / xstar.inf_norm();
}

// regular back-substitution without any regard for pivoting:
// this is to be used only when we do no pivoting: otherwise it's incorrect.
template<typename T>
nvector<T> backsub(const matrix<T>& U, const nvector<T>& b)
{
    nvector<T> solution(b.size());
    
    const auto n = U.rowCount();
    for (int i = n - 1; i >= 0; --i) {
        solution[i] = b[i];
        for (int j = i + 1; j < n; ++j) {
            solution[i] = solution[i] - U(i, j) * solution[j];
        }
        solution[i] /= U(i, i);
    }
    
    return solution;
}

// back-substitution overload that takes in a row pivot vector
// can be called if you don't use pivoting but you need to give it
// a vanilla row pivot vector (I think you can do this)
template<typename T>
nvector<T> backsub(const matrix<T>& U, const nvector<T>& b, std::vector<int> piv)
{
    nvector<T> solution(b.size());

    const auto n = U.rowCount();
    for (int i = n - 1; i >= 0; --i) {
        solution[piv[i]] = b[piv[i]];
        for (int j = i + 1; j < n; ++j) {
            solution[piv[i]] = solution[piv[i]] - U(piv[i], j) * solution[piv[j]];
        }
        solution[piv[i]] /= U(piv[i], i);
    }

    // translate the pivots back
    nvector<T> solution2(b.size());
    for (int i = 0; i < piv.size(); ++i)
    {
        solution2[i] = solution[piv[i]];
    }

    return solution2;
}

// overload of back-substitution for complete pivoting.
// When doing complete pivoting, one _must_ call this template function
// otherwise, nothing will look right (pretty sure)
template<typename T>
nvector<T> backsub(const matrix<T>& U, const nvector<T>& b, std::vector<int> piv, std::vector<int> cpiv)
{
    nvector<T> solution(b.size());

    const auto n = U.rowCount();
    for (int i = n - 1; i >= 0; --i) {
        solution[piv[i]] = b[piv[i]];
        for (int j = i + 1; j < n; ++j) {
            solution[piv[i]] = solution[piv[i]] - U(piv[i], cpiv[j]) * solution[piv[j]];
        }
        solution[piv[i]] /= U(piv[i], cpiv[i]);
    }

    // translate the pivots back
    nvector<T> solution2(b.size());
    for (int i = 0; i < piv.size(); ++i)
    {
        solution2[cpiv[i]] = solution[piv[i]];
    }

    return solution2;
}

template<typename T>
nvector<T> gaussian_no_pivoting(const matrix<T>& A, nvector<T> b)
{
    matrix<T> partial(A); // to row reduce to upper triangular
    
    // code here
    const auto nMinus1 = partial.colCount() - 1;
    const auto n = partial.rowCount();
    for (int i = 0; i < nMinus1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            // calculate the ratio
            auto ratio = partial(j, i) / partial(i, i);
            for (int k = i; k < n; ++k) {
                // modify matrix entry
                partial(j, k) = partial(j, k) - ratio * partial(i, k);
            }
            
            // modify result vector
            b[j] = b[j] - ratio * b[i];
            auto rjAfter = b[j];
        }
    }
    
    return backsub(partial, b);
}

template<typename T>
nvector<T> gaussian_partial_pivoting(const matrix<T>& A, nvector<T> b)
{
    matrix<T> partial(A); // to row reduce to upper triangular form
    std::vector<int> piv(A.rowCount()); // row pivot vector

    // init the pivot vector with the default (no pivots)
    for (int i = 0; i < piv.size(); ++i) {
        piv[i] = i;
    }

    const auto nMinus1 = partial.colCount() - 1;
    const auto n = partial.rowCount();
    for (int i = 0; i < nMinus1; ++i) {

        // do the pivot first
        T magnitude = 0;
        int index = -1;
        for (int j = i; j <= nMinus1; ++j) {
            if (std::abs(partial(piv[j], i)) > magnitude) {
                magnitude = std::abs(partial(piv[j], i));
                index = j;
            }
        }

        if (index != -1) {
            std::swap(piv[i], piv[index]);
        }

        for (int j = i + 1; j < n; ++j) {
            // calculate the ratio
            auto ratio = partial(piv[j], i) / partial(piv[i], i);
            for (int k = i; k < n; ++k) {
                // modify matrix entry
                partial(piv[j], k) = partial(piv[j], k) - ratio * partial(piv[i], k);
            }

            // modify result vector
            b[piv[j]] = b[piv[j]] - ratio * b[piv[i]];
        }
    }

    // call overloaded backsub for partial pivoting
    return backsub(partial, b, piv);
}

template<typename T>
nvector<T> gaussian_complete_pivoting(const matrix<T>& A, nvector<T> b)
{
    matrix<T> partial(A); // to row reduce to upper triangular form
    std::vector<int> piv(A.rowCount()); // row pivot vector
    std::vector<int> cpiv(A.colCount()); // column pivot vector

    // init the pivot vectors with the default (no pivots)
    for (int i = 0; i < piv.size(); ++i) {
        piv[i] = i;
        cpiv[i] = i;
    }

    const auto nMinus1 = partial.colCount() - 1;
    const auto n = partial.rowCount();
    for (int i = 0; i < nMinus1; ++i) {

        // do the pivot first
        T magnitude = 0;
        int row_index = -1;
        int col_index = -1;
        for (int j = i; j <= nMinus1; ++j) {
            for (int k = i; k <= nMinus1; ++k)
            {
                if (std::abs(partial(piv[j], piv[k])) > magnitude) {
                    magnitude = std::abs(partial(piv[j], cpiv[k]));
                    row_index = j;
                    col_index = k;
                }
            }
        }

        if (row_index != -1) {
            std::swap(piv[i], piv[row_index]);
        }

        if (col_index != -1) {
            std::swap(cpiv[i], cpiv[col_index]);
        }

        for (int j = i + 1; j < n; ++j) {
            // calculate the ratio
            auto ratio = partial(piv[j], cpiv[i]) / partial(piv[i], cpiv[i]);
            for (int k = i; k < n; ++k) {
                // modify matrix entry
                partial(piv[j], cpiv[k]) = partial(piv[j], cpiv[k]) - ratio * partial(piv[i], cpiv[k]);
            }

            // modify result vector
            b[piv[j]] = b[piv[j]] - ratio * b[piv[i]];
        }
    }

    // call overloaded backsub for complete pivoting
    return backsub(partial, b, piv, cpiv);
}

#endif /* matrix_hpp */
