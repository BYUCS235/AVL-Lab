CXXFLAGS = -Wall -g

main: main.cpp BST.cpp Node.cpp

test: test.cpp BST.cpp Node.cpp

clean:
	rm -f main
	rm -f test
	rm -f out_file*
	rm -f details.txt
	rm -f turnin.txt
