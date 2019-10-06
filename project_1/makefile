bottcher_prog1:	main

main:	main.o
	g++ -Wall -Wextra -pedantic -std=c++17 -g test.o -o test -Wl,--no-as-needed -ldl

main.o: test.cpp Deque.hpp
	g++ -g -c test.cpp

clean:
	rm -f *.o test
