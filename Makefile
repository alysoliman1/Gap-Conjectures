all: dynamical_bounds combinatorial_conditions clean

combinatorial_conditions: gen_tree

dynamical_bounds: tree json
	@gcc -c dynamical-bounds/lower_estimates.c -o lower_estimates.o
	@gcc lower_estimates.o json.o tree.o -o bins/lower_estimates

gen_tree: combinatorial-conditions/gen_tree.c combinatorial-conditions/gen_tree.h 
	@gcc -c combinatorial-conditions/gen_tree.c -o gen_tree.o

tree: tree/tree.c tree/tests.c tree/tree.h
	@gcc -c tree/tree.c -o tree.o
	@gcc -c tree/tests.c -o tree_tests.o
	@gcc tree_tests.o tree.o -o tree_tests
	@echo running tree unit tests ...
	@./tree_tests
	@rm tree_tests

json: json/json.c json/json.h
	@gcc -c json/json.c -o json.o

clean:
	@rm *.o
