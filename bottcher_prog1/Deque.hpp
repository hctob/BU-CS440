#ifndef BOTTCHER_DEQUE
#define BOTTCHER_DEQUE

#include <cstring> /*strlen() for Deque.type_name*/

#define str(a) #a

#define Deque_DEFINE(db)     \
    struct Deque_##db##_it;     \
    struct Deque_##db {        \
        db *elements;           \
        size_t _size = 0;          \
        unsigned int front_idx = 0;         \
        unsigned int back_idx = 0;          \
        /*Member Functions*/        \
        /*Modifiers*/               \
        void (*push_front)(Deque_##db *, db);                  \
        /*Capacity:*/                  \
        bool (*empty)(Deque_##db *);  \
        size_t (*max_size)(Deque_##db *);   \
        size_t (*size)(Deque_##db *);   \
        void (*resize)(Deque_##db *, db);   \
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
    void Deque_##db##_push_front(Deque_##db *ye, db value)  \
    {   \
        printf("test"); \
    }   \
    void resize(Deque_##db *ye, db value)   \
    {   \
        ye->elements = (db*)malloc((ye->size(ye) + 1) * sizeof(db)); \
    } \
    void Deque_##db##_ctor(Deque_##db *ye)  \
    {   \
        Deque_##db ndq;        \
        ndq._size = 0;  \
        ndq.empty = &empty; \
        ndq.size = &size; \
        ndq.push_front = &Deque_##db##_push_front; \
        /*ndq.type_name = (char*)malloc((strlen("Deque_"#db) + 1) * sizeof(char));*/ \
        strcpy(ndq.type_name, "Deque_"#db); \
        *ye = ndq;  \
    }

#endif
