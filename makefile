CXXFLAGS = -Wall -g

main: main.cpp AVL.cpp Node.cpp

test: test.cpp AVL.cpp Node.cpp

clean:
	rm -f main
	rm -f test
	rm -f out_file*
	rm -f details.txt
	rm -f turnin.txt
