//
// Created by yutony on 11/10/22.
//

#include <stack>
#include <algorithm>
#include "DPLL.h"

bool DPLL::solve(const cnf &input_cnf) {
    std::vector<cnf*> stack;
    cnf* current_cnf = new cnf(input_cnf);
    bool error = false;
    while (true) {
        if (error || !current_cnf->unit_propagation()) {
            delete current_cnf;
            if (stack.empty()) return false;
            current_cnf = stack.back();
            stack.pop_back();
            error = false;
            continue;
        }

        if (current_cnf->is_true()) break;

        unsigned int i =  current_cnf->get_atom();

        cnf* left_cnf = new cnf(*current_cnf);
        if (left_cnf->set_value(i, false)) {
            stack.push_back(left_cnf);
        } else delete left_cnf;
        if (!current_cnf->set_value(i, true)) {
            error = true;
        }
    }
    delete current_cnf;
    while (!stack.empty()) {
        delete stack.back();
        stack.pop_back();
    }
    return true;
}
