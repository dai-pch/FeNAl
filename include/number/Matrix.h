#ifndef NUMBER_MATRIX_H
#define NUMBER_MATRIX_H

#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

namespace Number{
    template <typename T> 
    class Matrix {
        template <typename T1> friend class Matrix;
    public:
        Matrix(size_t rows, size_t cols): _rows(rows), _cols(cols), _num(rows * cols){}
        Matrix(): Matrix(0, 0){}
        template <typename U>
        Matrix(size_t rows, size_t cols, const std::vector<U>& contents);
        template <typename U> Matrix(const Matrix<U>& m);

    public:
        T& operator()(size_t row, size_t col) {
            size_t posi = row*_cols + col;
            return _num[posi];
        }
        T operator()(size_t row, size_t col) const {
            size_t posi = row*_cols + col;
            return _num[posi];
        }
        Matrix transpose() const;
        Matrix reverse() const;
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
        friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
            os << "rows:" << m._rows << "; ";
            os << "cols:" << m._cols << "; " << std::endl << "[";
            for (T e: m._num) {
                os << e << ", ";
            }
            os << "]" << std::endl;
            return os;
        }

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

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Matrix<T>& m);

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
    Matrix<T> Matrix<T>::transpose() const {
        size_t r_rows = _cols;
        size_t r_cols = _rows;
        Matrix<T> res(_rows, _cols);
        for (size_t ii = 0;ii<r_rows;++ii)
            for (size_t jj = 0;jj<r_cols;++jj)
                res(ii, jj) += this->operator()(jj, ii);
        return res;
    }

} // namespace Number

#endif
