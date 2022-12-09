valgrind --tool=massif --massif-out-file="massif.out" ./dpll ./data/hanoi4.cnf
ms_print massif.out