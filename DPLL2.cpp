//
// Created by yutony on 11/10/22.
//

#include <stack>
#include <algorithm>
#include <iostream>
#include "DPLL2.h"

bool DPLL2::solve(const cnf2 &cnf) {
    std::vector<cnf2*> stack;
    cnf2* current_cnf = new cnf2(cnf);
    bool error = false;
    while (true) {
        if (error || !current_cnf->unit_propagation()) {
            delete current_cnf;
            if (stack.empty()) return false;
            current_cnf = stack.back();
            stack.pop_back();
//            std::cout << "backstage" << std::endl;
            error = false;
            continue;
        }

        if (current_cnf->is_true()) return true;
//        long i = std::max_element(current_cnf->interpretation.begin(), current_cnf->interpretation.end(), [](const auto &a, const auto &b) {
//            return a.status != -1 || (b.status == -1 && a.count < b.count);
//        }) - current_cnf->interpretation.begin();

        unsigned int i =  current_cnf->get_atom();
//        std::cout << i << " " << current_cnf->interpretation[i].status << std::endl;

        cnf2* left_cnf = new cnf2(*current_cnf);
        if (left_cnf->set_value(i, false)) {
            stack.push_back(left_cnf);
        } else delete left_cnf;
//        stack.push_back(new cnf2(*current_cnf));
//        stack.back()->set_value(i, false);
        if (!current_cnf->set_value(i, true)) {
            error = true;
        }
    }
    return false;
}

//bool DPLL2::solve(const cnf2 &cnf) {
//    std::stack<cnf2> stack;
//    stack.push(cnf);
//    bool error = false;
//    while (stack.empty()) {
//        if (!stack.top().unit_propagation()) {
//            stack.pop();
//        }
//
//        if (stack.top().is_true()) return true;
//
//        unsigned int i =  stack.top().get_atom();
//
//        stack.push(stack.top())
//    }
//    return false;
//}
