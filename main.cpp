#include <iostream>
#include <cstdlib>

#include "cnf2.h"
#include "DPLL2.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "1 argument expected";
        return 1;
    }
    std::string file_name = argv[1];
    std::cout << file_name << std::endl;
    std::ifstream is(file_name);
    cnf2 cnf = cnf2::parse(is);

    std::string result = DPLL2::solve(cnf) ? "SAT" : "UNSAT";
    std::cout << result << std::endl;

    const char *expect_result_c = std::getenv("RESULT");
    if (expect_result_c) {
        std::string expect_result = expect_result_c;
        if (expect_result != result)
            return 1;
    }
    return 0;
}
