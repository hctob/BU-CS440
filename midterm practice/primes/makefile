primes:	main

main:	main.o
	g++ -Wall -Wextra -pedantic -std=c++17 -g main.o -o primes

main.o: main.cpp primes.h
	g++ -g -c main.cpp

clean:
	rm -f *.o primes
