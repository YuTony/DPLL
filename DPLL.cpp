#include "DPLL.h"

#include <stack>
#include <algorithm>
#include <iostream>

bool DPLL::solve(const cnf &input_cnf) {
    std::stack<cnf> stack;
    stack.push(input_cnf);
    while (!stack.empty()) {
        cnf current_cnf = stack.top();
        stack.pop();
        current_cnf.unit_propagation();
        current_cnf.pure_literal_elimination();
        if (current_cnf.is_true()) return true;
        if (current_cnf.is_false()) {
            continue;
        }

//        current_cnf.clauses.sort([](const auto &a1, const auto &a2) {
//           return a2.size() > a1.size();
//        });

//        size_t i = current_cnf.clauses.begin()->begin()->first;

        size_t i =  std::find_if(current_cnf.interpretation.begin(), current_cnf.interpretation.end(), [](const auto &literal) {
            return (literal.status == -1 && literal.count > 0);
        }) - current_cnf.interpretation.begin();

        std::cout << i << std::endl;

        cnf right_cnf(current_cnf);
        right_cnf.set_value(i, true);
        stack.push(right_cnf);

        cnf left_cnf(current_cnf);
        left_cnf.set_value(i, false);
        stack.push(left_cnf);
    }
    return false;
}
