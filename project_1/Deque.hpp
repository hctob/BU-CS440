#ifndef BOTTCHER_DEQUE
#define BOTTCHER_DEQUE

#include <cstring> /*strlen() for Deque.type_name*/
#include <stdlib.h> /*qsort_r()*/

//https://codereview.stackexchange.com/questions/148303/implementation-of-stddeque
#define str(a) #a
#define START_SIZE 16

#define Deque_DEFINE(db)   \
    struct Deque_##db; \
    struct Deque_##db##_Iterator {   \
        db current_value;\
        unsigned long count; \
        bool decable;\
        Deque_##db *tracked_deque; \
        unsigned long position; \
        size_t size; \
        void (*inc)(Deque_##db##_Iterator *); \
        void (*dec)(Deque_##db##_Iterator *); \
        db &(*deref)(Deque_##db##_Iterator *); \
        \
    }; \
    struct Deque_##db {        \
        db *elements;           \
        size_t _size = 0;          \
        size_t capacity = START_SIZE;        \
        unsigned long front_idx = 0;         \
        unsigned long back_idx = 0;          \
        /*Member Functions:*/        \
        /*Modifiers:*/               \
        void (*push_front)(Deque_##db *, db);                  \
        void (*push_back)(Deque_##db *, db);                  \
        db (*pop_front)(Deque_##db *); \
        db (*pop_back)(Deque_##db *); \
        /*Capacity:*/                  \
        bool (*empty)(Deque_##db *);  \
        bool (*is_full)(Deque_##db *); \
        size_t (*max_size)(Deque_##db *);   \
        size_t (*size)(Deque_##db *);   \
        void (*resize)(Deque_##db *, long);   \
        /*Element Access:*/ \
        db &(*at)(Deque_##db *, unsigned long); \
        db &(*front)(Deque_##db *); \
        db &(*back)(Deque_##db *); \
        Deque_##db##_Iterator (*begin)(Deque_##db *); \
        Deque_##db##_Iterator (*end)(Deque_##db *); \
        void (*sort)(Deque_##db *, Deque_##db##_Iterator, Deque_##db##_Iterator); \
        void (*dtor)(Deque_##db *); \
        bool (*compare)(const db &, const db &); \
	int (*compare_sort)(const void*, const void*, void*); \
        void (*clear)(Deque_##db *); \
        char type_name[(strlen("Deque_"#db) + 1)]; \
        \
    };                                          \
    \
    /*Deque_##db##_Iterator Functions*/ \
    \
    void inc(Deque_##db##_Iterator * iterator) \
    { \
            iterator->position = (iterator->position + 1) % iterator->size; \
            iterator->current_value = iterator->tracked_deque->elements[iterator->position]; \
            iterator->count += 1; \
    } \
    void dec(Deque_##db##_Iterator * iterator) \
    { \
        if(iterator->decable) \
        { \
        iterator->position = (iterator->position - 1 + iterator->size) % iterator->size; \
        iterator->current_value = iterator->tracked_deque->elements[iterator->position]; \
        iterator->count -= 1; \
        } \
    } \
    db& deref(Deque_##db##_Iterator * iterator) \
    { \
        return iterator->tracked_deque->elements[iterator->position]; \
    } \
    bool Deque_##db##_Iterator_equal(Deque_##db##_Iterator i1, Deque_##db##_Iterator i2) \
    { \
            if(i1.count > i2.position) return true; \
            return false; \
    } \
    \
    Deque_##db##_Iterator begin(Deque_##db *ye) \
    { \
        Deque_##db##_Iterator newIt; \
        newIt.count = 0; \
        newIt.decable = true; \
        newIt.inc = &inc; \
        newIt.dec = &dec; \
        newIt.deref = deref; \
        newIt.tracked_deque = ye; \
        newIt.position = newIt.tracked_deque->front_idx; \
        newIt.current_value = ye->elements[newIt.position]; \
        newIt.size = newIt.tracked_deque->size(newIt.tracked_deque); \
        return newIt; \
    } \
    Deque_##db##_Iterator end(Deque_##db *ye) \
    { \
        Deque_##db##_Iterator newIt; \
        newIt.count = 0;\
        newIt.decable = true; \
        newIt.inc = &inc; \
        newIt.dec = &dec; \
        newIt.deref = deref; \
        newIt.tracked_deque = ye; \
        newIt.position = newIt.tracked_deque->back_idx + 1; \
        newIt.current_value = ye->elements[newIt.position]; \
        newIt.size = newIt.tracked_deque->size(newIt.tracked_deque); \
        return newIt; \
    } \
    /*Deque_##db Functions*/\
    \
    bool empty(Deque_##db *ye)\
    {   \
        return(ye->size(ye) == 0);                \
    }   \
    void clear(Deque_##db *ye) \
    { \
        ye->front_idx = 0; \
        ye->back_idx = 0; \
        ye->size = 0; \
    } \
    bool is_full(Deque_##db *ye)\
    {   \
        return((ye->back_idx + 1) % ye->capacity == ye->front_idx);                \
    }   \
    bool full(Deque_##db *ye) \
    { \
        return(ye->size(ye) == ye->capacity); \
    } \
    size_t size(Deque_##db *ye) \
    {\
            return ye->_size;   \
    }\
    void ensure_capacity(Deque_##db *ye) { \
        if(ye->size(ye) == ye->capacity) \
            { \
            /*printf("doubling capacity from %d to %d\n", (int)ye->capacity, (int)ye->capacity * 2); */ \
            db* temp = (db *)realloc(ye->elements, (2 * ye->capacity) * sizeof(db)); \
            for(unsigned long i = 0; i < ye->front_idx; i++) \
            { \
                temp[ye->size(ye)] = ye->elements[i]; \
                ye->capacity++; \
            } \
            ye->elements = temp; \
            ye->capacity *= 2; \
            ye->front_idx = 0; \
		ye->back_idx = ye->size(ye) - 1; \
        } \
    } \
    void push_front(Deque_##db *ye, db value)  \
    {   \
        /*ensure_capacity(ye);*/ \
        /*ye->front_idx = mod(ye->front_idx - 1, ye->capacity);*/ \
        /*printf("front_idx: %d\n", ye->front_idx); */ \
        /*db* temp = (db *)malloc((unsigned long)(ye->size(ye) + 1) * sizeof(db));*/ \
        /*ye->elements = (db *)realloc(&ye->elements, (ye->size(ye) + 1) * sizeof(db)); */ \
        /*ye->elements[ye->front_idx] = value;*/ \
        /*ye->_size++;*/ \
        ensure_capacity(ye); \
	    if(ye-size(ye) == 0) { /*empty*/ \
		    ye->front_idx = 0; \
		    ye->back_idx = 0; \
	    } \
	    else \
	    { \
	        ye->front_idx = (ye->front_idx - 1 + ye->capacity) % ye->capacity; \
	    } \
        ye->elements[ye->front_idx] = value; \
        ye->_size++; \
        /*printf("element pushed to front_idx: %d \n", (int)t_idx);*/ \
    }   \
    \
    void push_back(Deque_##db *ye, db value)  \
    {   \
        ensure_capacity(ye); \
        if(ye-size(ye) == 0) \
        { \
            ye->front_idx = 0; \
            ye->back_idx = 0; \
        } \
        else if(ye->size(ye) > 0)\
        { \
            ye->back_idx = (ye->back_idx + 1) % ye->capacity; \
        } \
	    /*ye->elements = (db *)realloc(&ye->elements, (ye->size(ye) + 1) * sizeof(db));*/ \
        ye->elements[ye->back_idx] = value; \
        ye->_size++; \
	    /*printf("element pushed to back_idx: %ld \n", t_idx);*/ \
    }   \
    db pop_back(Deque_##db * ye) \
    { \
        db YUS = ye->elements[ye->back_idx]; \
        if(ye->front_idx == ye->back_idx) \
        { \
            ye->front_idx = 0; \
            ye->back_idx = 0; \
        } \
        else \
        { \
            ye->back_idx = (ye->back_idx - 1 + ye->capacity) % ye->capacity; \
        } \
        ye->_size--; \
        return YUS; \
    } \
    db pop_front(Deque_##db *ye) \
    { \
        db YUS = ye->elements[ye->front_idx]; \
        if(ye->front_idx == ye->_size - 1) \
        { \
            ye->front_idx = 0; \
            ye->back_idx = 0; \
        } \
        else \
        { \
            ye->front_idx = (ye->front_idx + 1) % ye->capacity; \
        } \
        ye->_size--; \
        return YUS; \
    } \
    void resize(Deque_##db *ye, long value)   \
    {   \
        ye->elements = (db*)realloc(ye->elements, (ye->size(ye) + value) * sizeof(db)); \
    } \
    db& at(Deque_##db *ye, unsigned long idx) \
    { \
        if(idx == 0) \
        { \
            return ye->elements[ye->front_idx]; \
        } \
        else if(idx == ye->size(ye)) \
        { \
            return ye->elements[ye->back_idx]; \
        } \
        /*Should work - relative front + index % number of elements?*/ \
        return ye->elements[(ye->front_idx + idx) % ye->capacity]; \
    } \
    db& d_front(Deque_##db *ye) \
    { \
        return ye->elements[ye->front_idx]; \
    } \
    db& d_back(Deque_##db *ye) \
    { \
        /*return ye->elements[(ye->back_idx - 1) % ye->capacity];*/ \
        return ye->elements[ye->back_idx]; \
    } \
    bool Deque_##db##_equal(Deque_##db d1, Deque_##db d2) \
    { \
        /*printf("deq1: %d, deq2: %d\n", d1._size, d2._size); */ \
        if(d1._size != d2._size) return false; \
            for(unsigned long i = d1.front_idx; i <= d1.back_idx; i++) \
            { \
                if(d1.compare(d1.elements[i], d2.elements[i]) || d2.compare(d2.elements[i], d1.elements[i])) \
                { \
                    return false; \
                } \
            } \
        return true; \
    } \
    void dtor(Deque_##db *ye)      \
    {   \
        free(ye->elements);   \
    }   \
    int do_##db##_sort_compare(const void* d1, const void* d2, void* compar) \
    { \
	/*Deque_##db##_Iterator*/ \
	db db1 = *((db*)d1); \
	db db2 = *((db*)d2); \
	bool (*comparator)(const db &, const db &) = (bool(*)(const db &, const db &))compar; \
	if(comparator(db1, db2)) \
	{ \
		return -1; \
	} \
	else if(comparator(db2, db1)) \
	{ \
		return 1; \
	} \
	return 0; \
    } \
    void sort(Deque_##db *ye, Deque_##db##_Iterator i1, Deque_##db##_Iterator i2) \
	{ \
	 unsigned long diff; \
	 if(i1.position < i2.position) \
	 { \
		diff = (i2.position - i1.position); \
	 } \
	 else if(i2.position < i1.position){ \
		diff = (i1.position - i2.position) % ye->size(ye); \
	 } \
	 db* elementChunk = (db*)malloc(diff * sizeof(db)); \
	 unsigned long n_pos = 0; \
	 if(i1.position < i2.position) \
	 { \
		for(unsigned long i = i1.position; i < i2.position; i++) \
		{ \
			elementChunk[n_pos] = ye->elements[i]; \
			n_pos++; \
		} \
	 } \
	 else \
	 { \
		for(unsigned long i = i2.position; i < i1.position; i++) \
		{ \
			elementChunk[n_pos] = ye->elements[i]; \
			n_pos++; \
		} \
	 } \
	 qsort_r(elementChunk, diff, sizeof(db), do_##db##_sort_compare, (void*)ye->compare); \
	/* printf("element at 100 %d\n", elementChunk[100]); */\
	 n_pos = 0; \
	 if(i1.position < i2.position) \
	 { \
		for(unsigned long i = i1.position; i < i2.position; i++) \
		{ \
			ye->elements[i] = elementChunk[n_pos]; \
			n_pos++; \
		} \
	 } \
	 else \
	 { \
		for(unsigned long i = i2.position; i < i1.position; i++) \
		{ \
			ye->elements[i] = elementChunk[n_pos]; \
			n_pos++; \
		} \
	 } \
	free(elementChunk); \
	} \
    void Deque_##db##_ctor(Deque_##db *ye, bool comparator(const db &, const db &))  \
    {   \
        Deque_##db ndq;        \
        strcpy(ndq.type_name, "Deque_"#db); \
        ndq._size = 0;  \
        ndq.capacity = START_SIZE; \
        ndq.front_idx = 0; \
        ndq.back_idx = 0; \
        ndq.compare = comparator; \
	ndq.compare_sort = do_##db##_sort_compare; \
	    ndq.elements = (db *)malloc(ye->capacity * sizeof(db)); \
	    if(!ndq.elements) \
	    { \
		    printf("Malloc failed\n"); \
		    exit(1); \
	    } \
        ndq.empty = &empty; \
        ndq.begin = &begin; \
        ndq.end = &end; \
        ndq.clear = &clear; \
        ndq.dtor = &dtor; \
        ndq.size = &size; \
        ndq.push_front = &push_front; \
        ndq.push_back = &push_back; \
        ndq.pop_front = &pop_front; \
        ndq.pop_back = &pop_back; \
        ndq.at = at; \
        ndq.front = d_front; \
        ndq.back = d_back; \
        ndq.sort = &sort; \
        *ye = ndq; \
    }

#endif
