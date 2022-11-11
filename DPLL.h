//
// Created by yutony on 11/10/22.
//

#ifndef DPLL_DPLL_H
#define DPLL_DPLL_H


#include "cnf.h"

class DPLL {
public:
    static bool solve(const cnf &cnf);
};


#endif //DPLL_DPLL_H
