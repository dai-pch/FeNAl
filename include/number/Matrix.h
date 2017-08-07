#ifndef NUMBER_MATRIX_H
#define NUMBER_MATRIX_H

#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

namespace Number{
    template <typename T>
    class Matrix;

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Matrix<T>& m);

    template <typename T> 
    class Matrix {
        template <typename T1> friend class Matrix;
    public:
        Matrix(size_t rows, size_t cols): _rows(rows), _cols(cols), _num(rows * cols){}
        Matrix(): Matrix(0, 0){}
        template <typename U>
        Matrix(size_t rows, size_t cols, const std::vector<U>& contents);
        template <typename U> Matrix(const Matrix<U>& m);
        static Matrix ones(size_t n);

    public:
        T& operator()(size_t row, size_t col);
        T operator()(size_t row, size_t col) const;
        Matrix transpose() const;
        template <typename U = decltype((double)1.0/T())>
        Matrix<U> reverse() const;
        decltype((double)1 + T()) abs() const;
        template <typename U> bool operator==(const Matrix<U>& m) const;
        template <typename U> bool operator!=(const Matrix<U>& m) const;
        template <typename U> const Matrix<U>& operator=(const Matrix<U>& m);
        template <typename U, typename Tr = decltype(T() + U())> 
        Matrix<Tr> operator+(const Matrix<U>& m) const;
        template <typename U, typename Tr = decltype(T() + U())> 
        Matrix<Tr> operator-(const Matrix<U>& m) const;
        template <typename U, typename Tr = decltype(T() + U())> 
        Matrix<Tr> operator*(const Matrix<U>& m) const;

    public:
        friend std::ostream& operator<< <> (std::ostream& os, const Matrix<T>& m);

    private:
        template <typename U> void copy_from(const Matrix<U>& m);
        void copy_from(const Matrix& m);
        template <typename U> bool is_same_size(const Matrix<U>& m) const;

    private:
        size_t _rows;
        size_t _cols;
        std::vector<T> _num;
    }; // class Matrix

    template <typename T> template <typename U>
    Matrix<T>::Matrix(size_t rows, size_t cols, const std::vector<U> &contents) 
        : Matrix(rows, cols) {
        assert(contents.size() >= rows*cols);
        _num.clear();
        std::copy(contents.begin(), contents.begin() + rows*cols, std::back_inserter(_num));
    }
    template <typename T> template <typename U>
    Matrix<T>::Matrix(const Matrix<U>& m){
        this->copy_from(m);
    }
    template <typename T> Matrix<T> Matrix<T>::ones(size_t n){
        Matrix<T> res(n, n);
        for (size_t ii = 0;ii<n;++ii)
            res(ii, ii) = (T)1;
        return res;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
        os << "rows:" << m._rows << "; ";
        os << "cols:" << m._cols << "; " << std::endl << "[";
        for (const T& e: m._num) {
            os << e << ", ";
        }
        os << "]" << std::endl;
        return os;
    }

    template <typename T> template <typename U>
    void Matrix<T>::copy_from(const Matrix<U>& m){
        _rows = m._rows;
        _cols = m._cols;
        _num.clear();
        std::copy(m._num.begin(), m._num.end(), std::back_inserter(_num));
    }
    template <typename T>
    void Matrix<T>::copy_from(const Matrix& m){
        _rows = m._rows;
        _cols = m._cols;
        _num = m._num;
    }

    template <typename T>
    T& Matrix<T>::operator()(size_t row, size_t col) {
        size_t posi = row*_cols + col;
        return _num[posi];
    }
    template <typename T>
    T Matrix<T>::operator()(size_t row, size_t col) const {
        size_t posi = row*_cols + col;
        return _num[posi];
    }

    template <typename T> template <typename U>
    const Matrix<U>& Matrix<T>::operator=(const Matrix<U>& m) {
        this->copy_from(m);
        return m;
    }

    template <typename T> template <typename U> 
    bool Matrix<T>::is_same_size(const Matrix<U>& m) const {
        return (_rows == m._rows && _cols == m._cols);
    }

    template <typename T> template <typename U> 
    bool Matrix<T>::operator==(const Matrix<U>& m) const {
        if (_rows * _cols == 0 && m._rows * m._cols == 0)
            return true;
        if (!is_same_size(m))
            return false;
        for (size_t ii = 0;ii<_rows*_cols;++ii){
            if (_num[ii] != m._num[ii])
                return false;
        }
        return true;
    }

    template <typename T> template <typename U> 
    bool Matrix<T>::operator!=(const Matrix<U>& m) const {
        return !(*this == m);
    }

    template <typename T> template <typename U, typename Tr> 
    Matrix<Tr> Matrix<T>::operator+(const Matrix<U>& m) const {
        assert(this->is_same_size(m));
        Matrix<Tr> res(_rows, _cols);
        for (int ii = 0;ii<_rows*_cols;++ii)
            res._num[ii] = _num[ii] + m._num[ii];
        return res;
    }
    template <typename T> template <typename U, typename Tr> 
    Matrix<Tr> Matrix<T>::operator-(const Matrix<U>& m) const {
        assert(this->is_same_size(m));
        Matrix<Tr> res(_rows, _cols);
        for (int ii = 0;ii<_rows*_cols;++ii)
            res._num[ii] = _num[ii] - m._num[ii];
        return res;
    }
    
    template <typename T> template <typename U, typename Tr> 
    Matrix<Tr> Matrix<T>::operator*(const Matrix<U>& m) const {
        assert(this->_cols == m._rows);
        size_t r_rows = _rows;
        size_t r_cols = m._cols;
        size_t count = _cols;
        Matrix<Tr> res(_rows, _cols);
        for (size_t ii = 0;ii<r_rows;++ii)
            for (size_t jj = 0;jj<r_cols;++jj)
                for (size_t kk = 0;kk<count;++kk) {
                    res(ii, jj) += this->operator()(ii, kk) * m(kk, jj);
                }
        return res;
    }

    template <typename T>
    decltype((double)1 + T()) Matrix<T>::abs() const {
        decltype((double)1 + T()) res = 0;
        for (T e: _num)
            res += e * e;
        return std::sqrt(res);
    }

    template <typename T>
    Matrix<T> Matrix<T>::transpose() const {
        size_t r_rows = _cols;
        size_t r_cols = _rows;
        Matrix<T> res(_rows, _cols);
        for (size_t ii = 0;ii<r_rows;++ii)
            for (size_t jj = 0;jj<r_cols;++jj)
                res(ii, jj) += this->operator()(jj, ii);
        return res;
    }

    template <typename T> template <typename U>
    Matrix<U> Matrix<T>::reverse() const {
        assert(_rows == _cols);
        size_t dim = _rows;
        Matrix<U> res(*this);

        std::vector<size_t> row(dim);
        std::vector<size_t> col(dim);
        std::vector<bool> piv(dim, false);
        //select i-th main element 
        for (size_t i = 0;i<dim;++i) {
            //find max element
            size_t r, c;
            U max = 0;
            for (size_t j=0;j<dim;++j) {
                if (piv[j]) continue;
                for (size_t k=0;k<dim;++k) {
                    if (piv[k]) continue;
                    U temp = std::abs(res(j, k));
                    if (temp > max) {
                        r = j;
                        c = k;
                        max = temp;
                    }
                }
            }
            
            // main element can not be zero
            assert(max != 0);

            // swap rows to place main element in diagorithm
            if (r != c)
                for (size_t j=0;j<dim;++j)
                    std::swap(res(r, j), res(c, j));
            piv[c] = true;
            row[i] = r;
            col[i] = c;
            
            // zoom the row contents main element
            {
                U ratio = (double)1 / res(c, c);
                res(c, c) = 1;
                for (size_t j=0;j<dim;++j)
                    res(c, j) *= ratio;
            }

            // elimination
            for (size_t j=0;j<dim;++j) {
                if (j == c) continue;
                U ratio = res(j, c);
                res(j, c) = 0;
                for (size_t k=0;k<dim;++k)
                    res(j, k) -= ratio * res(c, k);
            }
        }

        // swap back
        for (int i=dim-1;i>=0;--i) {
            if (row[i] == col[i]) continue;
            for (size_t j=0;j<dim;++j)
                std::swap(res(j, row[i]), res(j, col[i]));
        }
        
        return res;
    }

} // namespace Number

#endif
