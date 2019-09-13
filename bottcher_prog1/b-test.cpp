//#include <dlfcn.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <random>
#include <unistd.h>
#include <string.h>
#include <algorithm>
#include <typeinfo>
#include "Deque.hpp"

struct MyClass {
    int id;
    char name[10];
};

bool
MyClass_less_by_id(const MyClass &o1, const MyClass &o2) {
    return o1.id < o2.id;
}

bool
MyClass_less_by_name(const MyClass &o1, const MyClass &o2) {
    return strcmp(o1.name, o2.name) < 0;
}

void
MyClass_print(const MyClass *o) {
    printf("%d\n", o->id);
    printf("%s\n", o->name);
}

Deque_DEFINE(MyClass)

int main(int argc, char** argv) {
    //Deque_int *de;
    //de->empty = &empty;
    //de->push_front = Deque_int_push_front;
    //std::cout << "test" << std::endl;
    Deque_MyClass deq;
    Deque_MyClass_ctor(&deq);
    printf("test 1\n");
    assert(deq.size(&deq) == 0);
    // size() should return a size_t.
    assert(typeid(std::size_t) == typeid(decltype(deq.size(&deq))));
    assert(deq.empty(&deq));
    printf("---- %s, %d\n", deq.type_name, (int) sizeof(deq.type_name));
    printf("yeet");
    return 0;
}
