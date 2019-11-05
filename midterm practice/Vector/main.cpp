#include <iostream>
#include <string>
#include "Vector.hpp"

int main(int argc, char** argv) {
	Vector<std::string> v;
	v.push_back("geeebs");
	v.push_back("in the");
	v.push_back("crib");
	
	Vector<std::string> v2;
	v2.push_back("jonathan");
	v2.push_back("slimer");
	v2.push_back("alert");
	v2.print();
	//std::cout << v[1] << std::endl;
	Vector<std::string> v3;
	v3 = v + v2;
	v3.print();
	return 0;
}
