smart:	main

main:	main.o
	g++ -Wall -Wextra -std=c++17 -g main.o -o smartptr

main.o: main.cpp SmartPtr.hpp
	g++ -g -c main.cpp

clean:
	rm -f *.o smartptr
