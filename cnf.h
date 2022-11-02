//
// Created by Anton on 21.10.2022.
//
#include <map>
#include <vector>
#include <fstream>
#include <list>

#ifndef DPLL_CNF_H
#define DPLL_CNF_H

struct ATOM {
    int status; // -1: undef; 0: false; 1: true;
    long count;
    int difference;
};

typedef std::vector<ATOM> INTERPRETATION;
typedef std::map<size_t, bool> CLAUSE;
typedef std::list<CLAUSE> CLAUSES;

class cnf {
public:
    cnf(int nClauses, int nAtoms) :
            interpretation(INTERPRETATION(nAtoms, { -1, 0, 0 })),
            clauses(CLAUSES(nClauses)) {}

    // cnf(const cnf &oldCNF);

    static cnf parse(std::ifstream &s);

    bool unit_propagation();

    bool pure_literal_elimination();

    [[nodiscard]] bool is_false() const;

    [[nodiscard]] bool is_true() const;

    bool set_value(long atom, bool value);

    INTERPRETATION interpretation;
    CLAUSES clauses;
};


#endif //DPLL_CNF_H
