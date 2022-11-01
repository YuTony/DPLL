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

        if (!current_cnf.unit_propagation())
            continue;

        if (!current_cnf.pure_literal_elimination())
            continue;
            
        if (current_cnf.is_true()) return true;
        if (current_cnf.is_false()) {
            continue;
        }

//        current_cnf.clauses.sort([](const auto &a1, const auto &a2) {
//           return a2.size() > a1.size();
//        });
//
//        size_t i = current_cnf.clauses.begin()->begin()->first;

//        size_t i =  std::find_if(current_cnf.interpretation.begin(), current_cnf.interpretation.end(), [](const auto &literal) {
//            return (literal.status == -1 && literal.count > 0);
//        }) - current_cnf.interpretation.begin();

        size_t i = std::max_element(current_cnf.interpretation.begin(), current_cnf.interpretation.end(), [](const auto &a, const auto &b) {
            return a.status != -1 || (b.status == -1 && a.count < b.count);
        }) - current_cnf.interpretation.begin();

        std::cout << i << " : ";
        for (const auto &l : current_cnf.interpretation) {
            std::cout << (l.status == -1 ? std::to_string(l.status) : " " + std::to_string(l.status)) << ' ';
        } std::cout << std::endl;

        cnf right_cnf(current_cnf);
        if (right_cnf.set_value(i, true))
            stack.push(right_cnf);

        cnf left_cnf(current_cnf);
        if (left_cnf.set_value(i, false))
            stack.push(left_cnf);
    }
    return false;
}
