//
// Created by yutony on 11/10/22.
//

#include "cnf2.h"
#include <sstream>
#include <algorithm>

cnf2::cnf2(const CLAUSES *clauses, int nAtoms) : clauses(clauses), interpretation(
        std::vector<ATOM>(nAtoms, {AtomStatus::Undefined, 0, 0})), clauses_count(clauses->size()) {}

cnf2 cnf2::parse(std::ifstream &is) {
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

    auto clauses = new CLAUSES(clauses_count);
    cnf2 newCNF = cnf2(clauses, var_count);

    int literal;
    auto clause = clauses->begin();
    while (is >> literal) {
        if (literal == 0) {
            if (clause->size() == 1)
                newCNF.single_clauses.push(clause - clauses->begin());
            newCNF.clauses_size.push_back(clause->size());
            clause++;
            continue;
        }
        newCNF.interpretation[std::abs(literal) - 1].count++;
        newCNF.interpretation[std::abs(literal) - 1].difference += literal > 0 ? 1 : -1;
        newCNF.interpretation[std::abs(literal) - 1].in_clauses.emplace_back(clause - clauses->begin(), literal > 0);
        clause->push_back({std::abs(literal) - 1, literal > 0});
    }
    return newCNF;
}

bool cnf2::is_true() const {
    return clauses_count == 0;
}

bool cnf2::set_value(unsigned int atom, bool value) {
    if (interpretation[atom].status != AtomStatus::Undefined) return true;
    interpretation[atom].status = static_cast<AtomStatus>(value);
    for (auto & in_clause : interpretation[atom].in_clauses) {
        if (clauses_size[in_clause.first] == 0) continue;
        if (in_clause.second == value) {
            clauses_size[in_clause.first] = 0;
            clauses_count--;
            for (const auto & c : (*clauses)[in_clause.first]) {
                interpretation[c.first].count--;
            }
        } else {
            if (clauses_size[in_clause.first] == 1)
                return false;
            clauses_size[in_clause.first]--;
            if (clauses_size[in_clause.first] == 1)
                single_clauses.push(in_clause.first);
        }
    }
    return true;
}

bool cnf2::unit_propagation() {
    while (!single_clauses.empty()) {
        auto single_clause = single_clauses.front();
        single_clauses.pop();
        auto literal = std::find_if((*clauses)[single_clause].begin(), (*clauses)[single_clause].end(), [&](const auto& literal) {
            return interpretation[literal.first].status == AtomStatus::Undefined;
        });
        if (literal == (*clauses)[single_clause].end()) continue;
        if (!set_value(literal->first, literal->second)) {
            return false;
        }
    }
    return true;
}

unsigned int cnf2::get_atom() {
//    return std::find_if(interpretation.begin(), interpretation.end(), [](const auto &literal) {
//        return (literal.status == -1 && literal.count > 0);
//    }) - interpretation.begin();
    return std::max_element(interpretation.begin(), interpretation.end(), [](const auto &a, const auto &b) {
        return a.status != -1 || (b.status == -1 && a.count < b.count);
    }) - interpretation.begin();
}

cnf2::cnf2(const cnf2 &cnf) : clauses(cnf.clauses), single_clauses(cnf.single_clauses), clauses_size(cnf.clauses_size), clauses_count(cnf.clauses_count) {
    interpretation = std::vector<ATOM>();
    for (const auto& in : cnf.interpretation) {
        if (in.status == AtomStatus::Undefined) {
            interpretation.push_back(in);
        } else {
            interpretation.push_back({in.status, in.count, in.difference});
        }
    }
}
