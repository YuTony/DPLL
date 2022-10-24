//
// Created by Anton on 24.10.2022.
//

#ifndef DPLL_DPLL_H
#define DPLL_DPLL_H

#include "cnf.h"


class DPLL {
public:
    static bool solve(const cnf &cnf);
};


#endif //DPLL_DPLL_H
