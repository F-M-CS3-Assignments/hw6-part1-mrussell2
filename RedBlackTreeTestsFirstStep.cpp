
#include <iostream>
#include <string>
#include <cassert>
#include <random>
#include "RedBlackTree.h"

/**
 * 
 * This "First Step" tests file is useful to get your project started
 * 
 * To run this code you only need to implement
 * 	+ The Constructor
 * 	+ The Insert() function to insert only one node
 * 	+ The ToInfixString() function to print a tree containing only one node
 * 
**/

using namespace std;

RedBlackTree::RedBlackTree() {
    root = nullptr;
    numItems = 0;
}

RedBlackTree::RedBlackTree(int newData) {
    Insert(newData);
}

RedBlackTree::RedBlackTree(const RedBlackTree& other) {
    root = CopyOf(other.root);
    numItems = other.numItems;
}
string RedBlackTree::ToInfixString(const RBTNode* node) {
    if (node == nullptr) return "";
    return ToInfixString(node->left) +
           " " + (node->color == COLOR_RED ? "R" : "B") + to_string(node->data) + " " +
           ToInfixString(node->right);
}

void RedBlackTree::Insert(int newData) {
    RBTNode* node = new RBTNode;
    node->data = newData;
    node->color = COLOR_RED;

    if (root == nullptr) {
        node->color = COLOR_BLACK;
        root = node;
        numItems++;
        return;
    }
    BasicInsert(node);

    if (node->parent != nullptr && node->parent->color == COLOR_RED) {
        InsertFixUp(node);
    }
    numItems++;
}
bool RedBlackTree::Contains(int data) const{
	return true;
}
//size_t RedBlackTree::Size() const {return numItems;};

int RedBlackTree::GetMin() const{
	return 0;
}
int RedBlackTree::GetMax() const{
	return 1;
}
void RedBlackTree::InsertFixUp(RBTNode *node){
	return;
}

void RedBlackTree::BasicInsert(RBTNode* node) {
	return; }

RBTNode *RedBlackTree::CopyOf(const RBTNode *node){
	return root;
}




void TestSimpleConstructor(){
	cout << "Testing Simple Constructor... " << endl;
	RedBlackTree rbt = RedBlackTree();
	//cout << "empty r-b-tree: " << rbt.ToInfixString() << endl;
	assert(rbt.ToInfixString() == "");
	
	cout << "PASSED!"<< endl << endl;
}


void TestInsertFirstNode(){
	cout << "Testing Insert One Node..." << endl;
	RedBlackTree rbt = RedBlackTree();
	rbt.Insert(30);
	//cout << "rbt: " << rbt.ToPrefixString() << endl;
	assert(rbt.ToInfixString() == " B30 ");

	cout << "PASSED!" << endl << endl;
}


int main(){	
	TestSimpleConstructor();
	TestInsertFirstNode();

	cout << "ALL TESTS PASSED!!" << endl;
	return 0;
}
