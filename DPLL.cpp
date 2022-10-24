//
// Created by Anton on 24.10.2022.
//

#include "DPLL.h"

#include <stack>
#include <algorithm>

bool DPLL::solve(const cnf &input_cnf) {
    std::stack<cnf> stack;
    stack.push(input_cnf);
    while (!stack.empty()) {
        cnf current_cnf = stack.top();
        stack.pop();
        current_cnf.unit_propagation();
        current_cnf.pure_literal_elimination();
        if (current_cnf.is_true()) return true;
        if (current_cnf.is_false()) continue;

        size_t i =  std::find_if(current_cnf.interpretation.begin(), current_cnf.interpretation.end(), [](const auto &literal) {
            return literal.status == -1;
        }) - current_cnf.interpretation.begin();

        cnf right_cnf(current_cnf);
        right_cnf.set_value(i, true);
        stack.push(right_cnf);

        cnf left_cnf(current_cnf);
        left_cnf.set_value(i, false);
        stack.push(left_cnf);
    }
    return false;
}
