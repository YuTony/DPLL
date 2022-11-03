//
// Created by Anton on 21.10.2022.
//
#include <map>
#include <vector>
#include <fstream>
#include <list>
#include <unordered_set>
#include <queue>
#include <set>
#include <tuple>

#ifndef DPLL_CNF_H
#define DPLL_CNF_H

typedef std::map<unsigned long, bool> CLAUSE;

typedef std::list<CLAUSE> CLAUSES;

struct ATOM {
    int status; // -1: undef; 0: false; 1: true;
    long count;
    int difference;
    std::unordered_set<const CLAUSE *> clauses;
};

typedef std::vector<ATOM> INTERPRETATION;

class cnf {
public:
    cnf(int nAtoms, const CLAUSES* clauses) :
            interpretation(INTERPRETATION(nAtoms, { -1, 0, 0 })),
            clauses(clauses),
            clauses_count(clauses->size()) {}

    static cnf parse(std::ifstream &s);

    bool unit_propagation();
    bool unit_propagation2();

    bool pure_literal_elimination();

    [[nodiscard]] bool is_false() const;

    [[nodiscard]] bool is_true() const;

    bool set_value(unsigned long atom, bool value);

    const CLAUSES *clauses;
    INTERPRETATION interpretation;
    std::map<const CLAUSE *, unsigned int> clauses_size;
    std::queue<const CLAUSE *> single_clauses;
    unsigned int clauses_count;

//    ~cnf() {
//        delete clauses;
//    }
};


#endif //DPLL_CNF_H
