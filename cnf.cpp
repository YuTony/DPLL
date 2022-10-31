//
// Created by Anton on 21.10.2022.
//
#include "cnf.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <cstdlib>
#include <algorithm>

// constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

cnf cnf::parse(std::ifstream &is) {
    if (!is.is_open())
        throw std::runtime_error("Failed to open file for reading");

    std::string line;
    size_t var_count, clauses_count;

    do {
        std::getline(is, line);
    } while (line[0] != 'p');

    std::istringstream iss(line);
    if (iss >> line && line == "p" && iss >> line && line == "cnf")
        iss >> var_count >> clauses_count;
    else
        throw std::runtime_error("parse error");

    cnf newCNF(clauses_count, var_count);

    int literal;
    auto clause = newCNF.clauses.begin();
    while (is >> literal) {
        if (literal == 0) {
            clause++;
            continue;
        }
        newCNF.interpretation[std::abs(literal) - 1].count++;
        newCNF.interpretation[std::abs(literal) - 1].difference += literal > 0 ? 1 : -1;
        clause->insert({ std::abs(literal) - 1, literal > 0 });
    }
    return newCNF;
}

bool cnf::isSAT() {
    return true;
}

bool cnf::unit_propagation() {
    while (true) {
        auto it = std::find_if(clauses.begin(), clauses.end(), [](const auto &clause) {
            return clause.size() == 1;
        });
        if (it == clauses.end())
            break;
        if (!set_value(it->begin()->first, it->begin()->second))
            return false;
    }
    return true;
}

bool cnf::pure_literal_elimination() {
    for (int i = 0; i < interpretation.size(); ++i) {
        if (interpretation[i].count > 0 && interpretation[i].count == std::abs(interpretation[i].difference)) {
            if (!set_value(i, interpretation[i].difference > 0)) 
            return false;
//            std::cout << "detected\n";
        }
    }
    return true;
}

bool cnf::set_value(size_t atom, bool value) {
    interpretation[atom].status = value;
    auto clause = clauses.begin();
    while (clause != clauses.end()) {
        auto literal = clause->find(atom);
        if (literal != clause->end()) {
            if (literal->second == value){
                for (auto const &literal_it : *clause) {
                    interpretation[literal_it.first].count--;
                    interpretation[literal_it.first].difference -= literal_it.second ? 1 : -1;
                }
                clause = clauses.erase(clause);
                continue;
            } else {
                if (clause->size() == 1)
                    return false;
                interpretation[literal->first].count--;
                interpretation[literal->first].difference -= literal->second ? 1 : -1;
                clause->erase(literal);
            }
        }
        clause++;
    }
    return true;
}

bool cnf::is_false() const {
    return std::find_if(clauses.begin(), clauses.end(), [](const auto &clause) {
        return clause.size() == 0;
    }) != clauses.end();
}

bool cnf::is_true() const {
    return clauses.empty();
}

// cnf::cnf(const cnf &oldCNF) : cnf(oldCNF.clauses.size(), oldCNF.interpretation.size()) {
//     auto it1 = oldCNF.interpretation.begin();
//     auto it2 = interpretation.begin();
//     while (it1 != oldCNF.interpretation.end() && it2 != interpretation.end()) {
//         it2->difference = it1->difference;
//         it2->count = it1->count;
//         it2->status = it1->status;
//         it1++; it2++;
//     }
//     auto it3 = oldCNF.clauses.begin();
//     auto it4 = clauses.begin();
//     while (it3 != oldCNF.clauses.end() && it4 != clauses.end()) {
//         std::copy(it3->begin(), it3->end(), std::inserter(*it4, it4->end()));
//         it3++; it4++;
//     }
// }

