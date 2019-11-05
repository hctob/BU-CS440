#include <iostream>
#include "SmartPtr.hpp"
#include "../String/String.hpp"

int main(int argc, char** argv) {
	SmartPtr<String> p(new String("geebers"));
	return 0;
}
