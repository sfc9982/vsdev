#pragma once

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

namespace mino2357 {
    template<typename T = double>
    class vector {
    public:
        std::vector<T> vec;
        vector(size_t size) noexcept;
        inline T norm() noexcept;
    };

    template<typename T>
    vector<T>::vector(size_t size) noexcept {
        vec.resize(size);
    }

    template<typename T>
    inline T vector<T>::norm() noexcept {
        T ret = 0.0;
        for (size_t i = 0; i < vec.size(); ++i) {
            ret += this->vec[i] * this->vec[i];
        }
        return sqrt(ret);
    }

    template<typename T>
    inline vector<T> operator+(const vector<T> &x, const vector<T> &y) noexcept {
        vector<T> ret(x.vec.size());
        for (size_t i = 0; i < ret.vec.size(); ++i) {
            ret.vec[i] = x.vec[i] + y.vec[i];
        }
        return ret;
    }

    template<typename T>
    inline vector<T> operator-(const vector<T> &x, const vector<T> &y) noexcept {
        vector<T> ret(x.vec.size());
        for (size_t i = 0; i < ret.vec.size(); ++i) {
            ret.vec[i] = x.vec[i] - y.vec[i];
        }
        return ret;
    }

    template<typename T>
    inline T operator*(const vector<T> &x, const vector<T> &y) noexcept {
        T ret = 0.0;
        for (size_t i = 0; i < x.vec.size(); ++i) {
            ret += x.vec[i] * y.vec[i];
        }
        return ret;
    }

    template<typename T>
    inline vector<T> operator*(const T a, const vector<T> &y) noexcept {
        vector<T> ret(y.vec.size());
        for (size_t i = 0; i < ret.vec.size(); ++i) {
            ret.vec[i] = a * y.vec[i];
        }
        return ret;
    }
} // namespace mino2357