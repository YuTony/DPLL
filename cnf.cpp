//
// Created by Anton on 21.10.2022.
//
#include "cnf.h"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>

// constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

cnf cnf::parse(std::ifstream &is) {
    if (!is.is_open())
        throw std::runtime_error("Failed to open file for reading");

    std::string line;
    int var_count, clauses_count;

    do {
        std::getline(is, line);
    } while (line[0] != 'p');

    std::istringstream iss(line);
    if (iss >> line && line == "p" && iss >> line && line == "cnf")
        iss >> var_count >> clauses_count;
    else
        throw std::runtime_error("parse error");

    CLAUSES* clauses = new CLAUSES(clauses_count);
    cnf newCNF = cnf(var_count, clauses);

    int literal;
    auto clause = clauses->begin();
    while (is >> literal) {
        if (literal == 0) {
            if (clause->size() == 1)
                newCNF.single_clauses.push(&(*clause));
            newCNF.clauses_size.insert({ &(*clause), clause->size() });
            clause++;
            continue;
        }
        newCNF.interpretation[std::abs(literal) - 1].count++;
        newCNF.interpretation[std::abs(literal) - 1].difference += literal > 0 ? 1 : -1;
        newCNF.interpretation[std::abs(literal) - 1].clauses.insert(&(*clause));
        clause->insert({ std::abs(literal) - 1, literal > 0 });
    }
    return newCNF;
}

bool cnf::unit_propagation() {
    int i = 0;
    while (true) {
        i++;
        auto it = std::find_if(clauses->begin(), clauses->end(), [](const auto &clause) {
            return clause.size() == 1;
        });
        if (it == clauses->end()) {
            break;
        }
        if (!set_value(it->begin()->first, it->begin()->second)) {
//            std::cout << "unit_propagation " << i << std::endl;
            return false;
        }
    }
//    std::cout << "unit_propagation " << i << std::endl;
    return true;
}

bool cnf::unit_propagation2() {
    while (!single_clauses.empty()) {
        const CLAUSE* single_clause = single_clauses.front();
        single_clauses.pop();
        if (!set_value(single_clause->begin()->first, single_clause->begin()->second)) {
            return false;
        }
    }
    return true;
}

bool cnf::pure_literal_elimination() {
    for (int i = 0; i < interpretation.size(); ++i) {
        if (interpretation[i].count > 0 && interpretation[i].count == std::abs(interpretation[i].difference)) {
            if (!set_value(i, interpretation[i].difference > 0)) 
            return false;
        }
    }
    return true;
}

bool cnf::set_value(unsigned long atom, bool value) {
//    std::cout << "delete " << atom << std::endl;
    if (interpretation[atom].status != -1) {
        return true;
    }

    interpretation[atom].status = value;
    auto clause_with_atom = interpretation[atom].clauses.begin();
    while (clause_with_atom != interpretation[atom].clauses.end()) {
        auto clause_with_atom_ref = *clause_with_atom;
        auto literal_with_atom = (*clause_with_atom)->find(atom);
        if (literal_with_atom->second == value) {
            for (auto const &literal_it : **clause_with_atom) {
                if (interpretation[literal_it.first].status == -1 && interpretation[literal_it.first].count) {
                    interpretation[literal_it.first].count--;
                    interpretation[literal_it.first].difference -= literal_it.second ? 1 : -1;
                    if (literal_it.first != atom)
                        interpretation[literal_it.first].clauses.erase(*clause_with_atom);
                }
            }
            clauses_size[*clause_with_atom] = 0;
//            if (!single_clauses.empty() && single_clauses.front()->begin()->first == atom) {
//                single_clauses.pop();
//            }
            clauses_count--;
            clause_with_atom = interpretation[atom].clauses.erase(clause_with_atom);
        } else {
            if (clauses_size[*clause_with_atom] == 1) {
                return false;
            }
            interpretation[literal_with_atom->first].count--;
            interpretation[literal_with_atom->first].difference -= literal_with_atom->second ? 1 : -1;
//            interpretation[literal_with_atom->first].clauses.erase(*clause_with_atom);
            if (clauses_size[*clause_with_atom] != 0)
                clauses_size[*clause_with_atom]--;
            if (clauses_size[*clause_with_atom] == 1) {
                single_clauses.push(*clause_with_atom);
//                std::cout << "add " << (*clause_with_atom)->begin()->first << std::endl;
            }
            clause_with_atom = interpretation[atom].clauses.erase(clause_with_atom);
        }
    }
    return true;
}

bool cnf::is_false() const {
    return std::find_if(clauses->begin(), clauses->end(), [](const auto &clause) {
        return clause.size() == 0;
    }) != clauses->end();
}

bool cnf::is_true() const {
    return clauses_count == 0;
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

