# zadani: Hledání více vzorků v textu I
# autor: Martin Holoubek
# datum: 25. 11. 2015

CF=-Ofast -flto -Wno-sign-compare -std=c++11 -Wall -pedantic 
CF+=-fopenmp -fstrict-aliasing -mfpmath=sse -mavx 
CF+=-mpc32 -funsafe-math-optimizations -ffast-math 
# CF+=-ftree-vectorize -fopt-info-vec-missed 
CF+=-ftree-vectorize -fopt-info-vec -g
CF+=-falign-functions=1 -falign-jumps=1 -falign-loops=1 -falign-labels=1
CF+=-fassociative-math -freciprocal-math -fno-trapping-math  -funroll-loops -ftree-loop-im

all: task_2 task_3

task_1: makefile task_1.cpp
	g++ $(CF) -march=native task_1.cpp -o task_1

task_2: makefile task_2.cpp
	g++ $(CF) -march=native task_2.cpp -o task_2

task_3: makefile task_3.cpp
	g++ $(CF) -march=native task_3.cpp -o task_3

task_1_star: makefile task_1.cpp
	g++ $(CF) -march=core-avx-i task_1.cpp -o task_1_star

task_2_star: makefile task_2.cpp
	g++ $(CF) -march=core-avx-i task_2.cpp -o task_2_star

clear:
	rm -f task_1 task_2 task_3 task_1_star task_2_star task_3_star callgrind* cachegrind* nanoTask_1.sh.* nanoTask_2.sh.* nanoTask_3.sh.*
