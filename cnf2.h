//
// Created by yutony on 11/10/22.
//

#ifndef DPLL_CNF2_H
#define DPLL_CNF2_H

#include <vector>
#include <tuple>
#include <set>
#include <fstream>
#include <queue>

enum AtomStatus {
    Undefined = -1,
    False = 0,
    True = 1
};

struct ATOM {
    AtomStatus status;
    unsigned int count;
    int difference;
    std::vector<std::pair<unsigned int, bool>> in_clauses;
};

typedef std::vector<std::vector<std::pair<unsigned int, bool>>> CLAUSES;

class cnf2 {
public:
    cnf2(const CLAUSES* clauses, int nAtoms);

    cnf2(const cnf2& cnf);

    static cnf2 parse(std::ifstream &is);

    [[nodiscard]] bool is_true() const;

    bool set_value(unsigned int atom, bool value);

    bool unit_propagation();

    unsigned int get_atom();

    bool pure_literal_elimination();

//private:
    const CLAUSES* clauses;
    std::vector<ATOM> interpretation;
    std::queue<unsigned int> single_clauses;
    std::vector<unsigned int> clauses_size;
    unsigned int clauses_count;
};


#endif //DPLL_CNF2_H
