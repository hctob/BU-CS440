#ifndef PRIMES_H
#define PRIMES_H

#include <iostream>
#include <vector>
#define mod(a,b) a % b
namespace cs540 {
    class Primes {
    public:
        class Iterator {
        public:
            unsigned long index;
            std::vector<unsigned long> values;
            Iterator() = delete;
            Iterator(unsigned int start, std::vector<unsigned long> pvals) : index(start), values(pvals) {}

                        Iterator& operator=(const Iterator &old)
                        {
                            index = old.index;
                            return *this;
                        }

                        Iterator &operator++()
                        {
                            index++;
                            return *this;
                        }

                        Iterator operator++(int)
                        {
                            Iterator iter(*this);
                            ++(*this);
                            return iter;
                        }
                        Iterator &operator--()
                        {
                            index--;
                            return *this;
                        }
                        Iterator operator--(int)
                        {
                            Iterator iter(*this);
                            --(*this);
                            return iter;
                        }
                        unsigned long operator*() const
                        {
                            return values.at(index);
                        }
                        friend bool operator!=(const Iterator &a, const Iterator &b)
                        {
                            return !(a == b);
                        }
                        friend bool operator==(const Iterator &a, const Iterator &b)
                        {
                            return (a.index == b.index);
                        }
        };
        Iterator begin() {
            return Iterator(0, primes);
        }
        Iterator end() {
            return Iterator(primes.size(), primes);
        }
        Primes() : primes(100, 0) {}
        Primes(unsigned long upper_bound) {
            for(unsigned long n = 2; n <= upper_bound; n++) {
                unsigned long divisor = n / 2;
                bool prime = true;
                for(unsigned long i = 2; i <= divisor; i++) {
                    if(n % i == 0) { prime = false; break; }
                }
                if(prime) primes.push_back(n);
            }
        }
    private:
        std::vector<unsigned long> primes;
    };
}
#endif
