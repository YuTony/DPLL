//
// Created by Anton on 21.10.2022.
//
#include "cnf.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <cstdlib>
#include <algorithm>

constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

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

void cnf::unit_propagation() {
    while (true) {
        auto it = std::find_if(clauses.begin(), clauses.end(), [](const auto &clause) {
            return clause.size() == 1;
        });
        if (it == clauses.end())
            break;
        set_value(it->begin()->first, it->begin()->second);
    }
}

void cnf::pure_literal_elimination() {
    for (int i = 0; i < interpretation.size(); ++i) {
        if (interpretation[i].count > 0 && interpretation[i].count == interpretation[i].difference) {
            set_value(i, interpretation[i].difference > 0);
        }
    }
}

void cnf::set_value(size_t atom, bool value) {
    for (auto clause = clauses.begin(); clause != clauses.end(); clause++) {
        auto literal = clause->find(atom);
        if (literal != clause->end()) {
            if (literal->second == value){
                for (auto const &literal_it : *clause) {
                    interpretation[literal_it.first].count--;
                    interpretation[literal_it.first].difference += literal_it.second ? 1 : -1;
                }
                clauses.erase(clause);
            } else {
                interpretation[literal->first].count--;
                interpretation[literal->first].difference += literal->second ? 1 : -1;
                clause->erase(literal);
            }
        }
    }
}

bool cnf::is_false() const {
    return std::find_if(clauses.begin(), clauses.end(), [](const auto &clause) {
        return clause.size() == 0;
    }) != clauses.end();
}

bool cnf::is_true() const {
    return clauses.empty();
}

