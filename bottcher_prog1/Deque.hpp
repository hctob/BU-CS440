#ifndef BOTTCHER_DEQUE
#define BOTTCHER_DEQUE
#include <iostream>

#define Deque_DEFINE(db)     \
    struct Deque_##db##_it;     \
    struct Deque_##db {        \
        db *elements;           \
        size_t _size = 0;          \
        int front = -1;         \
        unsigned int back = 0;          \
                                    \
        /*Modifiers*/               \
        void (*push_front)(Deque_##db *, db);                            \
        /*Capacity:*/                  \
        bool (*empty)(Deque_##db *);  \
        size_t (*max_size)(Deque_##db *);   \
        size_t (*size)(Deque_##db *);   \
        void (*resize)(Deque_##db *, db);   \
    };                                          \
    bool empty(Deque_##db *ye)\
    {   \
        return(ye->size(ye) == 0);                \
    }   \
    size_t size(Deque_##db *ye) \
    {\
            return ye->_size;   \
    }\
    void Deque_##db##_push_front(Deque_##db *ye, db value)  \
    {   \
        std::cout << __func__ << std::endl; \
    }   \
    void resize(Deque_##db *ye, db value)   \
    {   \
        ye->elements = (db*)malloc((ye->size(ye) + 1) * sizeof(db)); \
    }

#endif
