#ifndef BOTTCHER_DEQUE
#define BOTTCHER_DEQUE

#include <cstring> /*strlen() for Deque.type_name*/

#define str(a) #a
#define MAXIMUM_SIZE 100

#define Deque_DEFINE(db)     \
    struct Deque_##db##_it;     \
    struct Deque_##db {        \
        db *elements;           \
        size_t _size = 0;          \
        unsigned int front_idx = -1;         \
        unsigned int back_idx = 0;          \
        /*Member Functions:*/        \
        /*Modifiers:*/               \
        void (*push_front)(Deque_##db *, db);                  \
        /*Capacity:*/                  \
        bool (*empty)(Deque_##db *);  \
        size_t (*max_size)(Deque_##db *);   \
        size_t (*size)(Deque_##db *);   \
        void (*resize)(Deque_##db *, db);   \
        /*Element Access:*/ \
        db &(*at)(Deque_##db *, unsigned int); \
        db &(*front)(Deque_##db); \
        db &(*back)(Deque_##db); \
        char type_name[(strlen("Deque_"#db) + 1)]; \
    };                                          \
    void Deque_##db##_dtor(Deque_##db *ye)      \
    {   \
        free(ye);   \
    }   \
    bool empty(Deque_##db *ye)\
    {   \
        return(ye->size(ye) == 0);                \
    }   \
    size_t size(Deque_##db *ye) \
    {\
            return ye->_size;   \
    }\
    void push_front(Deque_##db *ye, db value)  \
    {   \
        if(ye->front_idx == -1) \
        { \
            ye->front_idx++; \
            ye->back_idx = 0; \
        } \
        else \
        { \
            if(ye->back_idx == MAXIMUM_SIZE - 1) \
            { \
                ye->back_idx = 0; \
            } \
            else  \
            { \
                ye->back_idx += 1; \
            } \
        } \
        ye->elements[ye->back_idx] = value; \
        ye->_size++; \
    }   \
    void resize(Deque_##db *ye, db value)   \
    {   \
        ye->elements = (db*)malloc((ye->size(ye) + 1) * sizeof(db)); \
    } \
    db &at(Deque_##db *ye, unsigned int idx) \
    { \
        return ye->elements[idx]; \
    } \
    db &_front(Deque_##db *ye) \
    { \
        return ye->elements[ye->front_idx]; \
    } \
    db &_back(Deque_##db *ye) \
    { \
        return ye->elements[ye->back_idx]; \
    } \
    void Deque_##db##_ctor(Deque_##db *ye)  \
    {   \
        Deque_##db ndq;        \
        ndq._size = 0;  \
        ndq.front_idx = -1; \
        ndq.back_idx = 0; \
        ndq.empty = &empty; \
        ndq.size = &size; \
        ndq.push_front = &push_front; \
        ndq.at = at; \
        ndq.front = _front; \
        ndq.back = _back; \
        /*ndq.type_name = (char*)malloc((strlen("Deque_"#db) + 1) * sizeof(char));*/ \
        strcpy(ndq.type_name, "Deque_"#db); \
        *ye = ndq;  \
    }

#endif
