.PHONY: all clean

all: clean
	g++ -g -Wall -Werror -pedantic-errors -std=c++11 -o ava *.cpp

clean:
	rm -f ava 
