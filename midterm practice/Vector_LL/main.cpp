#include <iostream>
#include <string>
#include "Vector_LL.h"

int main() {
    //std::cout << "Hello, World!" << std::endl;
    Vector<std::string> v;
    v.push_back("geeebs.");
    v.push_front("love");
    v.push_front("REALLY");
    v.push_front("I");
    v.print_forward();
    //delete v;
    //v.pop_front();
    //v.print_forward();

    Vector<std::string> v2;
    v2.push_back("So");
    v2.push_back("does");
    v2.push_back("Gabriel");
    v2.print_forward();
    Vector<std::string> v3;
    v3 = v + v2;
    v3.print_forward();
    //std::cout << v3.pop_front() << std::endl;
    v3.pop_front();
    v3.print_forward();
    return 0;
}