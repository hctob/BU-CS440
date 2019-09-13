bottcher_prog1:	main

main:	main.o
	g++ -g b-test.o -o b-test

main.o: b-test.cpp Deque.hpp
	g++ -g -c b-test.cpp


clean:
	rm -f *.o b-test
