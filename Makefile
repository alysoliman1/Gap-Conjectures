all: dynamic_bounds clean

dynamic_bounds: tree json
	@gcc -c dynamicbounds/lower_bound_estimates.c -o lower_bounds.o
	@gcc lower_bounds.o json.o tree.o -o bins/dynamic_bounds

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