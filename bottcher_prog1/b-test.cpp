#include <iostream>
#include <assert.h>
#include "Deque.hpp"

Deque_DEFINE(int);
int main(int argc, char** argv) {
    //Deque_int *de;
    //de->empty = &empty;
    //de->push_front = Deque_int_push_front;
    //std::cout << "test" << std::endl;
    Deque_int *de = (Deque_int*)malloc(sizeof(Deque_int));

    de->empty = empty;
    //std::cout << "test 2\n";
    de->push_front = Deque_int_push_front;
    de->size = size;
    std::cout << de->size << std::endl;
    //Deque_int *r_de = &de;
    //(de->empty(de) == 0);
    de->push_front(de, 5);
    //std::cout << std::boolalpha << "IsEmpty? "  << de->empty(de) << std::endl;

    return 0;
}
