#ifndef NUMBER_Mat_H
#define NUMBER_Mat_H

#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

namespace Number{
    template <typename T, size_t M, size_t N>
    class Mat;

    template <typename T, size_t M, size_t N>
    std::ostream& operator<<(std::ostream& os, const Mat<T, M, N>& m);

    template <typename T, size_t M, size_t N> 
    class Mat {
        template <typename T1, size_t M1, size_t N1> friend class Mat;
        static_assert(M > 0 && N > 0);
    public:
        Mat() {}
        template <typename U>
        Mat(const std::vector<U>& contents);
        template <typename U> Mat(const Mat<U, M, N>& m);
        static Mat ones();

    public:
        T& operator()(size_t row, size_t col);
        T operator()(size_t row, size_t col) const;
        Mat transpose() const;
        template <typename U = decltype((double)1.0/T())>
        Mat<U, M, N> reverse() const;
        decltype((double)1 + T()) abs() const;
        template <typename U> bool operator==(const Mat<U, M, N>& m) const;
        template <typename U> bool operator!=(const Mat<U, M, N>& m) const;
        template <typename U> const Mat<U, M, N>& operator=(const Mat<U, M, N>& m);
        template <typename U, typename Tr = decltype(T() + U())> 
        Mat<Tr, M, N> operator+(const Mat<U, M, N>& m) const;
        template <typename U, typename Tr = decltype(T() + U())> 
        Mat<Tr, M, N> operator-(const Mat<U, M, N>& m) const;
        template <typename U, size_t Q, typename Tr = decltype(T() + U())> 
        Mat<Tr, M, Q> operator*(const Mat<U, N, Q>& m) const;

    public:
        friend std::ostream& operator<< <> (std::ostream& os, const Mat<T, M, N>& m);
        size_t rows() const;
        size_t cols() const;

    private:
        template <typename U> 
        void copy_from(const Mat<U, M, N>& m);
        void copy_from(const Mat& m);
        template <typename U>
        void copy_from(const std::vector<U>& m);

    private:
        constexpr static size_t Dim = M * N;
        T _num[Dim] = {0};
    }; // class Mat

    template <typename T, size_t M, size_t N> template <typename U>
    Mat<T, M, N>::Mat(const std::vector<U> &contents) {
        this->copy_from(contents);
    }
    template <typename T, size_t M, size_t N> template <typename U>
    Mat<T, M, N>::Mat(const Mat<U, M, N>& m){
        this->copy_from(m);
    }
    template <typename T, size_t M, size_t N> Mat<T, M, N> Mat<T, M, N>::ones(){
        Mat<T, M, N> res;
        for (size_t ii = 0;ii<std::min(M, N);++ii)
            res(ii, ii) = (T)1;
        return res;
    }

    template <typename T, size_t M, size_t N>
    std::ostream& operator<<(std::ostream& os, const Mat<T, M, N>& m) {
        os << "rows:" << M << "; ";
        os << "cols:" << N << "; " << std::endl << "[";
        for (const T& e: m._num) {
            os << e << ", ";
        }
        os << "]" << std::endl;
        return os;
    }

    template <typename T, size_t M, size_t N> template <typename U>
    void Mat<T, M, N>::copy_from(const Mat<U, M, N>& m){
        for (int ii = 0; ii < Dim; ++ii)
            _num[ii] = m._num[ii];
    }
    template <typename T, size_t M, size_t N>
    void Mat<T, M, N>::copy_from(const Mat& m){
        for (int ii = 0; ii < Dim; ++ii)
            _num[ii] = m._num[ii];
    }

    template <typename T, size_t M, size_t N> template <typename U>
    void Mat<T, M, N>::copy_from(const std::vector<U>& m) {
        for (int ii = 0; ii < Dim; ++ii)
            _num[ii] = m[ii];
    }

    template <typename T, size_t M, size_t N>
    T& Mat<T, M, N>::operator()(size_t row, size_t col) {
        size_t posi = row * N + col;
        return _num[posi];
    }
    template <typename T, size_t M, size_t N>
    T Mat<T, M, N>::operator()(size_t row, size_t col) const {
        size_t posi = row * N + col;
        return _num[posi];
    }

    template <typename T, size_t M, size_t N> template <typename U>
    const Mat<U, M, N>& Mat<T, M, N>::operator=(const Mat<U, M, N>& m) {
        this->copy_from(m);
        return m;
    }

    template <typename T, size_t M, size_t N> template <typename U> 
    bool Mat<T, M, N>::operator==(const Mat<U, M, N>& m) const {
        for (size_t ii = 0;ii<Dim;++ii){
            if (_num[ii] != m._num[ii])
                return false;
        }
        return true;
    }

    template <typename T, size_t M, size_t N> template <typename U> 
    bool Mat<T, M, N>::operator!=(const Mat<U, M, N>& m) const {
        return !(*this == m);
    }

    template <typename T, size_t M, size_t N> template <typename U, typename Tr> 
    Mat<Tr, M, N> Mat<T, M, N>::operator+(const Mat<U, M, N>& m) const {
        Mat<Tr, M, N> res;
        for (int ii = 0;ii<Dim;++ii)
            res._num[ii] = _num[ii] + m._num[ii];
        return res;
    }
    template <typename T, size_t M, size_t N> template <typename U, typename Tr> 
    Mat<Tr, M, N> Mat<T, M, N>::operator-(const Mat<U, M, N>& m) const {
        Mat<Tr, M, N> res;
        for (int ii = 0;ii<Dim;++ii)
            res._num[ii] = _num[ii] - m._num[ii];
        return res;
    }
    
    template <typename T, size_t M, size_t N> template <typename U, size_t Q, typename Tr> 
    Mat<Tr, M, Q> Mat<T, M, N>::operator*(const Mat<U, N, Q>& m) const {
        Mat<Tr, M, Q> res;
        for (size_t ii = 0;ii<M;++ii)
            for (size_t jj = 0;jj<Q;++jj)
                for (size_t kk = 0;kk<N;++kk) {
                    res(ii, jj) += this->operator()(ii, kk) * m(kk, jj);
                }
        return res;
    }

    template <typename T, size_t M, size_t N>
    decltype((double)1 + T()) Mat<T, M, N>::abs() const {
        decltype((double)1 + T()) res = 0;
        for (T e: _num)
            res += e * e;
        return std::sqrt(res);
    }

    template <typename T, size_t M, size_t N>
    Mat<T, M, N> Mat<T, M, N>::transpose() const {
        Mat<T, M, N> res;
        for (size_t ii = 0;ii<M;++ii)
            for (size_t jj = 0;jj<N;++jj)
                res(ii, jj) += this->operator()(jj, ii);
        return res;
    }

    template <typename T, size_t M, size_t N> template <typename U>
    Mat<U, M, N> Mat<T, M, N>::reverse() const {
        static_assert(M == N);
        size_t dim = M;
        Mat<U, M, M> res(*this);

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

    template <typename T, size_t M, size_t N> size_t Mat<T, M, N>::rows() const{
        return M;
    }
    template <typename T, size_t M, size_t N> size_t Mat<T, M, N>::cols() const{
        return N;
    }

} // namespace Number

#endif
