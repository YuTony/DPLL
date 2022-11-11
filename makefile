all:
	g++ -std=c++17 -Wpedantic -Werror -O2 main.cpp cnf.cpp cnf.h DPLL.cpp DPLL.h -o dpll