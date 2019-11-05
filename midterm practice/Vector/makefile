tree:	main

main:	main.o
	g++ -Wall -Wextra -std=c++17 -g main.o -o vector

main.o: main.cpp Vector.hpp
	g++ -g -c main.cpp

clean:
	rm -f *.o vector
