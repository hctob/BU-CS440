#include "String.hpp"
#include <iostream>
#include <cstring>

int main() {
	String s = String("geebers");
	String s1 = String("geebriel");
	std::cout << s << std::endl;
	//s = s1;
	std::cout << s1 << std::endl;
	//delete s;
	char* derp = s.toCharPtr();
	String new_s = s + s1;
	std::cout << new_s << std::endl;
	//delete s1;
	//delete &new_s;
	return 0;
}
