cmake_minimum_required(VERSION 3.22)
project(DPLL)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -Wpedantic -Werror -O2")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})


#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        # Specify the commit you depend on and update it regularly.
#        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
#)
## For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(DPLL main.cpp cnf.cpp cnf.h DPLL.cpp DPLL.h)
#target_link_libraries(DPLL gtest_main)
add_test(NAME test20 COMMAND DPLL ${CMAKE_SOURCE_DIR}/data/uf20-01.cnf)
set_property(TEST test20 PROPERTY ENVIRONMENT "RESULT=SAT")

add_test(NAME test50 COMMAND DPLL ${CMAKE_SOURCE_DIR}/data/uuf50-0291.cnf)
set_property(TEST test50 PROPERTY ENVIRONMENT "RESULT=UNSAT")

# add_test(NAME testcom COMMAND DPLL ${CMAKE_SOURCE_DIR}/data/complex.dimacs)
# set_property(TEST testcom PROPERTY ENVIRONMENT "RESULT=SAT")

add_test(NAME test75 COMMAND DPLL ${CMAKE_SOURCE_DIR}/data/uf75-01.cnf)
set_property(TEST test75 PROPERTY ENVIRONMENT "RESULT=SAT")

add_test(NAME test175 COMMAND DPLL ${CMAKE_SOURCE_DIR}/data/uf175-087.cnf)
set_property(TEST test175 PROPERTY ENVIRONMENT "RESULT=SAT")

add_test(NAME hanoi4 COMMAND DPLL ${CMAKE_SOURCE_DIR}/data/hanoi4.cnf)
set_property(TEST hanoi4 PROPERTY ENVIRONMENT "RESULT=SAT")

#add_executable(Google_Tests_run tests.cpp)
#target_link_libraries(Google_Tests_run gtest gtest_main)
#add_test(NAME example_test COMMAND Google_Tests_run)
