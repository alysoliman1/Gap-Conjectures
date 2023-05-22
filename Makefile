all: dynamical_bounds combinatorial_conditions clean

combinatorial_conditions: bin gen_tree

dynamical_bounds: tree json
	@gcc -c dynamical-bounds/lower_estimates.c -o lower_estimates.o
	@gcc lower_estimates.o json.o tree.o -o binaries/lower_estimates

gen_tree: combinatorial-conditions/gen_tree.c combinatorial-conditions/gen_tree.h 
	@gcc -c combinatorial-conditions/gen_tree.c -o gen_tree.o

bin: utils/bin/bin.c
	@gcc -c utils/bin/bin.c -o bin.o
	@gcc -c utils/bin/tests.c -o bin_tests.o
	@gcc bin_tests.o bin.o -o bin_tests
	@echo running bin module unit tests ...
	@./bin_tests
	@rm bin_tests	 

tree: utils/tree/tree.c utils/tree/tests.c utils/tree/tree.h
	@gcc -c utils/tree/tree.c -o tree.o
	@gcc -c utils/tree/tests.c -o tree_tests.o
	@gcc tree_tests.o tree.o -o tree_tests
	@echo running tree module unit tests ...
	@./tree_tests
	@rm tree_tests

json: utils/json/json.c utils/json/json.h
	@gcc -c utils/json/json.c -o json.o

clean:
	@rm *.o
