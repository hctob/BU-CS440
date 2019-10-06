# you can modify this makefile

# you can modify CXXFLAGES and CFLAGS
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -Wl,--no-as-needed -ldl
CFLAGS = -Wall -Wextra -pedantic -std=c11 -Wl,--no-as-needed -ldl

CXX = g++
CC = gcc

TESTFILES_C = $(wildcard *.c)
TESTFILES_CC = $(wildcard *.cc)
TESTFILES_CPP = $(wildcard *.cpp)

NAMES = $(patsubst %.cc, %, $(TESTFILES_CC)) $(patsubst %.cpp, %, $(TESTFILES_CPP)) $(patsubst %.c, %, $(TESTFILES_C))


all: $(NAMES)

clean:
	-rm $(NAMES)

help:
	# make: compile all c and cpp files
	# make example: compile example.c*
	# you can modify this makefile
