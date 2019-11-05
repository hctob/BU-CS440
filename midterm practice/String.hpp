#ifndef BOTTCHER_STRING_H
#define BOTTCHER_STRING_H
#include <iostream>
#include <cstring>

class String {
public:
	String() {}
	String(const char* &str) : internal_str(str) {}
	String(const String &str) : internal_str(str.internal_str) {}
private:
	char* internal_str;
	
};
#endif
