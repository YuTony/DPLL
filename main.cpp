#include <iostream>
#include <cstdlib>

#include "cnf.h"
#include "DPLL.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "1 argument expected";
        return 1;
    }
    std::string file_name = argv[1];
    std::ifstream is(file_name);
    cnf cnf = cnf::parse(is);

    std::string result = DPLL::solve(cnf) ? "SAT" : "UNSAT";
    std::cout << result << std::endl;

    cnf.release();

    const char *expect_result_c = std::getenv("RESULT");
    if (expect_result_c) {
        std::string expect_result = expect_result_c;
        if (expect_result != result)
            return 1;
    }
    return 0;
}
