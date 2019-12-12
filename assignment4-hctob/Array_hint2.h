//
// Created by dbottch1 on 12/12/19.
//

#ifndef ASSIGNMENT4_HCTOB_ARRAY_HINT2_H
#define ASSIGNMENT4_HCTOB_ARRAY_HINT2_H
#include <iostream>

class Array_hint2 {
private:
    template <int N> struct Helper;
public:
    inline Helper<2> operator[](size_t i);
};

template <int N>
struct Array::Helper {
    Helper(size_t i) : n(i) {}
    Helper<N-1> operator[](size_t i) {
        return Helper<N-1>(i * n);
    }
    const size_t n;
};

template<>
struct Array::Helper<0> {
    void operator[](size_t i) {
        std::cout << "0: " << n* i << std::endl;
    }
    const size_t n;
};

inline Array::Helper<2> {
    Array::operator[](size_t i) {
        return Helper<2>(i);
    }
};

int main() {
    a[2][3][4][5];
}


#endif //ASSIGNMENT4_HCTOB_ARRAY_HINT2_H
