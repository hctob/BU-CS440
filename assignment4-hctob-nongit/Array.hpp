#ifndef ASSIGNMENT_FOUR_BOTTCHER_HPP
#define ASSIGNMENT_FOUR_BOTTCHER_HPP

#include <stdio.h>
#include <utility>
#include <stdlib.h>
#include <vector>
namespace cs540 {
    template<typename T, size_t... Dims>
    class Array {
        assert(Dims > 0); //cause error if array dims are not valid
    private:
        //T

    public:
        class FirstDimensionMajorIterator {
        public:
            size_t count;

            FirstDimensionMajorIterator() : count(0) {
            }
            FirstDimensionMajorIterator(const FirstDimensionMajorIterator& it) = default;
            FirstDimensionMajorIterator& operator=(const FirstDimensionMajorIterator& it) = default;
            bool operator==(const FirstDimensionMajorIterator& b) {
                return count == b.count;
            }
            bool operator!=(const FirstDimensionMajorIterator& b) {
                return !(*this == b);
            }
            FirstDimensionMajorIterator& operator++() {
                count += 1;
                return *this;
            }
            FirstDimensionMajorIterator& operator++(int) {
                auto it = *this;
                this->operator++();
                return it;
            }
            T& operator*() const {
                //return
            }
        };
        class LastDimensionMajorIterator {
        public:
            size_t count;
            LastDimensionMajorIterator() : count(0) {

            }
            LastDimensionMajorIterator(const LastDimensionMajorIterator& it) = default;
            LastDimensionMajorIterator& operator=(const LastDimensionMajorIterator& it) = default;
            bool operator==(const LastDimensionMajorIterator& b) {
                return count == b.count;
            }
            bool operator!=(const LastDimensionMajorIterator& b) {
                return !(*this == b);
            }
            LastDimensionMajorIterator& operator++() {
                count += 1;
                return *this;
            }
            LastDimensionMajorIterator& operator++(int) {
                auto it = *this;
                this->operator++();
                return it;
            }
            T& operator*() const {
                //return
            }
        };
        using ValueType = T;
        //using FirstDimensionMajorIterator
        //using LastDimensionMajorIterator
        Array() {

        }
        Array(const Array &arr) {

        }
        template <typename U>
        Array(const Array<U, Dims...> &arr) {

        }
        Array& operator=(const Array &) {

            return *this;
        }
        template <typename U>
        Array& operator=(const Array<U, Dims...> &arr) {

            return *this;
        }
        /*T &operator[](size_t index) {
            return internal_array
        }*/
        FirstDimensionMajorIterator fmbegin() {
            return FirstDimensionMajorIterator();
        }
        FirstDimensionMajorIterator fmend() {
            return FirstDimensionMajorIterator();
        }
        LastDimensionMajorIterator lmbegin() {
            return LastDimensionMajorIterator();
        }
        LastDimensionMajorIterator lmend() {
            return LastDimensionMajorIterator();
        }

    };
}
#endif