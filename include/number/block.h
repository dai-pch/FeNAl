#ifndef NUMBER_BLOCK_H
#define NUMBER_BLOCK_H

#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

namespace Number{
    template <typename T>
    class Block {
    public:
        Block() = default;
        Block(size_t size): data(size) {}
        Block& operator=(const Block&) = delete;

    public:
        Block clone() const {
            return Block(&this);
        }
        T& operator[](size_t n) {
            return data[n];
        }
        size_t size() const {
            return data.size();
        }

    private:
        vector<T> data;

    private:

        Block(const Block& src): data(src.data) {}
    }; // class Block

} // namespace Number

#endif // NUMBER_BLOCK_H
