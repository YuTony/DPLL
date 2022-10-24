//
// Created by Anton on 21.10.2022.
//
#include <unordered_map>
#include <vector>
#include <fstream>
#include <list>

#ifndef DPLL_CNF_H
#define DPLL_CNF_H

struct ATOM {
    int status; // -1: undef; 0: false; 1: true;
    size_t count;
    size_t difference;
};

typedef std::vector<ATOM> INTERPRETATION;
typedef std::unordered_map<size_t, bool> CLAUSE;
typedef std::list<CLAUSE> CLAUSES;

class cnf {
public:
    cnf() :
            interpretation(INTERPRETATION()),
            clauses(CLAUSES()) {}

    cnf(const cnf &oldCNF) = default;

    static cnf parse(std::ifstream &s);

    void unit_propagation();

    void pure_literal_elimination();

    bool is_false();

    bool is_true();

    void set_value(size_t atom, bool value);

    bool isSAT();

private:
    INTERPRETATION interpretation;
    CLAUSES clauses;
};


#endif //DPLL_CNF_H
