#ifndef NUMBER_MATRIX_H
#define NUMBER_MATRIX_H

#include <cassert>
#include <vector>
#include <algrithm>

namespace Number{
    template<typename T> class Matrix {
    public:
        Matrix(size_t rows, size_t cols): _rows(rows), _cols(cols), _num(rows * cols){}
        Matrix(): Matrix(0, 0){}
        template<typename U>
        Matrix(size_t rows, size_t cols, std::vector<U>& contents);
        template<typename U> Matrix(Matrix<U>& m);

    public:
        Matrix& transpose();
        Matrix& reverse();
        Matrix& operator==(Matrix& m);
        Matrix& operator!=(Matrix& m);
        template<typename U> Matrix& operator=(Matrix<U>& m);
        template<typename U> Matrix& operator+(Matrix<U>& m);
        template<typename U> Matrix& operator-(Matrix<U>& m);
        template<typename U> Matrix& operator*(Matrix<U>& m);

    private:
        template<typename U> void copy_from(Matrix<U>& m);
        void copy_from(Matrix& m);

    private:
        size_t _rows;
        size_t _cols;
        std::vector<T> _num;
    }

    template<typename T, typename U>
    Matrix<T>::Matrix(size_t rows, size_t cols, std::vector<U>& contents){
        assert(contents.size() >= rows*cols);
        std::copy(contents.begin(), contents.begin() + rows*cols, std::back_inster(_num));
    }
    template<typename T, typename U> 
    Matrix<T>::Matrix(Matrix<U>& m){
        *this = m;
    }

    template<typename T, typename U>
    void Matrix<T>::copy_from(Matrix<U>& m){
        assert(m.size() >= rows*cols);
        _rows = m._rows;
        _cols = m._cols;
        std::copy(m._num.begin(), m._num.begin() + rows*cols, std::back_inster(_num));
    }
    template<typename T>
    void Matrix<T>::copy_from(Matrix& m){
        _rows = m._rows;
        _cols = m._cols;
        _num = m._num;
    }

    template<typename T, typename U>
    Matrix<U>& Matrix<T>::operator=(Matrix<U>& m){
        this->copy_from(m);
        return m;
    }

} // namespace Number

#endif
