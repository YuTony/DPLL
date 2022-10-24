//
// Created by Anton on 21.10.2022.
//
#include <iostream>
#include <limits>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include "cnf.h"

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

    cnf newCNF;

    int literal;
    auto clause = newCNF.clauses.begin();
    while (is >> literal) {
        if (literal == 0) {
            clause++;
            continue;
        }
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
        for (auto clause = clauses.begin(); clause != clauses.end(); clause++) {
            if (clause == it) continue;
            auto literal = clause->find(it->begin()->first);
            if (literal != clause->end()) {
                if (literal->second == it->begin()->second){
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
        interpretation[it->begin()->first].count--;
        interpretation[it->begin()->first].difference += it->begin()->second ? 1 : -1;
        clauses.erase(it);
    }
}

void cnf::pure_literal_elimination() {
    for (auto const &atom : interpretation) {
        if (atom.count > 0 && atom.count == atom.difference) {

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
