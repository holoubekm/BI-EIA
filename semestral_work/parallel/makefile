# zadani: Hledání více vzorků v textu I
# autor: Martin Holoubek
# datum: 5. 10. 2015

CF=-Wno-sign-compare -std=c++11 -g -Wall -pedantic -march=native -fopenmp
# CF+=-DDEBUG
# CF+=-DINSTR
# O=bin_no_ins
 O=bin_ins

all: new_chunked_parallel
	
new_chunked_parallel: makefile header.cpp footer.cpp new_chunked_parallel.cpp
	g++ $(CF) -O3 new_chunked_parallel.cpp -o $(O)/new_chunked_parallel
	
clear:
	rm -f new_chunked_parallel bin_no_ins/new_chunked_parallel bin_ins/new_chunked_parallel callgrind* cachegrind*

