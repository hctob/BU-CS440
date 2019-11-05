tree:	main

main:	main.o
	g++ -Wall -Wextra -std=c++17 -g main.o -o tree

main.o: main.cpp Tree.hpp
	g++ -g -c main.cpp

clean:
	rm -f *.o tree
