all:
	g++ -g -Wall RedBlackTree.cpp RedBlackTreeTests.cpp -o rbt-tests

run:
	./rbt-tests
