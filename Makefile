all: utils contexts bfs clean

bfs: bfs/node.cpp bfs/node.hpp bfs/bfs.cpp contexts utils 
	g++ -std=c++11 -c bfs/bfs.cpp -o bfs.obj
	g++ -std=c++11 -c bfs/node.cpp -o node.obj
	g++ -std=c++11 bfs.obj node.obj contexts.obj bucket.obj -o binaries/bfs

contexts: contexts/contexts.cpp contexts/contexts.hpp
	g++ -c -std=c++11 contexts/contexts.cpp -o contexts.obj

utils: utils/bucket.cpp utils/bucket.hpp utils/tests.cpp
	g++ -c -std=c++11 utils/bucket.cpp -o bucket.obj
	g++ -c -std=c++11 utils/tests.cpp -o utils_tests.obj
	g++ -std=c++11 bucket.obj utils_tests.obj -o utils_tests
	./utils_tests
	rm utils_tests 
clean:
	rm *.obj