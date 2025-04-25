
#include <iostream>
#include <string>
#include <cassert>
#include <random>
#include "RedBlackTree.h"

using namespace std;
//basic constructor
RedBlackTree::RedBlackTree() {
    root = nullptr;
    numItems = 0;
}
//constructur with int input
RedBlackTree::RedBlackTree(int newData) {
    Insert(newData);
}
//constructor with RBT input
RedBlackTree::RedBlackTree(const RedBlackTree& other) {
    root = CopyOf(other.root);
    numItems = other.numItems;
}
//function to allow to print tree
string RedBlackTree::ToInfixString(const RBTNode* node) {
    if (node == nullptr) return "";
    return ToInfixString(node->left) +
           " " + (node->color == COLOR_RED ? "R" : "B") + to_string(node->data) + " " +
           ToInfixString(node->right);
}
//function to print prefix to a string
string RedBlackTree::ToPrefixString(const RBTNode* node) {
    if (node == nullptr) return "";
    return std::string(" ") +
       (node->color == COLOR_RED ? "R" : "B") + to_string(node->data) + " " +
       ToPrefixString(node->left) +
       ToPrefixString(node->right);
}
// function to print postfix to a string
string RedBlackTree::ToPostfixString(const RBTNode* node) {
    if (node == nullptr) return "";
    return ToPostfixString(node->left) +
        ToPostfixString(node->right) +
        " " + (node->color == COLOR_RED ? "R" : "B") + to_string(node->data) + " ";
}
//function to insert new number
void RedBlackTree::Insert(int newData) {
    //ensure no duplicate nodes added
    if (Contains(newData)) {
        throw invalid_argument("Duplicate data not allowed in RedBlackTree.");
    }
    RBTNode* node = new RBTNode; //create new node
    node->data = newData; // set data to the input
    node->color = COLOR_RED;

    if (root == nullptr) { // if the root is null set color to black and sent root to input node
        node->color = COLOR_BLACK;
        root = node;
        numItems++;
        return;
    }
    BasicInsert(node); // if root is not empty use BasicInsert to insert node
    if (node->parent != nullptr && node->parent->color == COLOR_RED) {
        InsertFixUp(node);
    }
    numItems++;
}
//function to check if data is in the tree
bool RedBlackTree::Contains(int data) const {
    return Get(data) != nullptr;
}

RBTNode* RedBlackTree::Get(int data) const {
    RBTNode* curr = root;
    while (curr) {
        if (curr->data == data) return curr;
        curr = (data < curr->data) ? curr->left : curr->right;
    }
    return nullptr;
}

RBTNode* RedBlackTree::GetUncle(RBTNode* node) const {
    if (!node || !node->parent || !node->parent->parent) return nullptr;
    RBTNode* gp = node->parent->parent;
    return (gp->left == node->parent) ? gp->right : gp->left;
}

bool RedBlackTree::IsLeftChild(RBTNode* node) const {
    return node && node->parent && node->parent->left == node;
}

bool RedBlackTree::IsRightChild(RBTNode* node) const {
    return node && node->parent && node->parent->right == node;
}

int RedBlackTree::GetMin() const {
    if (!root) throw runtime_error("Tree is empty");
    RBTNode* node = root;
    while (node->left) node = node->left;
    return node->data;
}


int RedBlackTree::GetMax() const {
    if (!root) throw runtime_error("Tree is empty");
    RBTNode* node = root;
    while (node->right) node = node->right;
    return node->data;
}

void RedBlackTree::InsertFixUp(RBTNode* new_node) {
    RBTNode* parent = new_node->parent;
    RBTNode* uncle = GetUncle(new_node);
    RBTNode* grand_parent = (parent) ? parent->parent : nullptr;

    if (uncle == nullptr || uncle->color == COLOR_BLACK) {
        if (grand_parent != nullptr)
            grand_parent->color = COLOR_RED;

        if (IsLeftChild(new_node) && IsLeftChild(parent)) {
            RightRotate(grand_parent);
            parent->color = COLOR_BLACK;
        }
        else if (IsRightChild(new_node) && IsRightChild(parent)) {
            LeftRotate(grand_parent);
            parent->color = COLOR_BLACK;
        }
        else if (IsLeftChild(new_node) && IsRightChild(parent)) {
            RightRotate(parent);
            LeftRotate(grand_parent);
            new_node->color = COLOR_BLACK;
            parent->color = COLOR_RED;
        }
        else if (IsRightChild(new_node) && IsLeftChild(parent)) {
            LeftRotate(parent);
            RightRotate(grand_parent);
            new_node->color = COLOR_BLACK;
            parent->color = COLOR_RED;
        }
        else {
            throw invalid_argument("Impossible state during InsertFixUp");
        }
    }
    else if (uncle->color == COLOR_RED) {
        parent->color = COLOR_BLACK;
        uncle->color = COLOR_BLACK;
        if (grand_parent != nullptr) {
            if (root != grand_parent) {
                grand_parent->color = COLOR_RED;
                if (grand_parent->parent != nullptr &&
                    grand_parent->parent->color == COLOR_RED) {
                    InsertFixUp(grand_parent);
                }
            }
        }
    }

    root->color = COLOR_BLACK;
}

void RedBlackTree::BasicInsert(RBTNode* node) {
    RBTNode* current = root;
    RBTNode* parent = nullptr;

    while (current != nullptr) {
        parent = current;
        if (node->data < current->data)
            current = current->left;
        else
            current = current->right;
    }

    node->parent = parent;
    if (node->data < parent->data)
        parent->left = node;
    else
        parent->right = node;
}

RBTNode* RedBlackTree::CopyOf(const RBTNode* node) {
    if (!node) return nullptr;
    RBTNode* copy = new RBTNode(*node);
    copy->left = CopyOf(node->left);
    copy->right = CopyOf(node->right);
    if (copy->left) copy->left->parent = copy;
    if (copy->right) copy->right->parent = copy;
    return copy;
}

// rotations
void RedBlackTree::LeftRotate(RBTNode* n) {
    RBTNode* rChild = n->right;
    RBTNode* rlGranChild = rChild->left;

    if (IsLeftChild(n)) {
        n->parent->left = rChild;
    }
    else if (IsRightChild(n)) {
        n->parent->right = rChild;
    }
    else {
        root = rChild;
    }

    rChild->parent = n->parent;
    rChild->left = n;
    n->parent = rChild;
    n->right = rlGranChild;
    if (rlGranChild)
        rlGranChild->parent = n;
}
void RedBlackTree::RightRotate(RBTNode* n) {
    RBTNode* lChild = n->left;
    RBTNode* lrGranChild = lChild->right;

    if (IsLeftChild(n)) {
        n->parent->left = lChild;
    }
    else if (IsRightChild(n)) {
        n->parent->right = lChild;
    }
    else {
        root = lChild;
    }

    lChild->parent = n->parent;
    lChild->right = n;
    n->parent = lChild;
    n->left = lrGranChild;
    if (lrGranChild)
        lrGranChild->parent = n;
}




// void TestSimpleConstructor(){
// 	cout << "Testing Simple Constructor... " << endl;
// 	RedBlackTree rbt = RedBlackTree();
// 	//cout << "empty r-b-tree: " << rbt.ToInfixString() << endl;
// 	assert(rbt.ToInfixString() == "");
	
// 	cout << "PASSED!"<< endl << endl;
// }


// void TestInsertFirstNode(){
// 	cout << "Testing Insert One Node..." << endl;
// 	RedBlackTree rbt = RedBlackTree();
// 	rbt.Insert(30);
// 	//cout << "rbt: " << rbt.ToPrefixString() << endl;
// 	assert(rbt.ToInfixString() == " B30 ");

// 	cout << "PASSED!" << endl << endl;
// }


// int main(){	
// 	TestSimpleConstructor();
// 	TestInsertFirstNode();

// 	cout << "ALL TESTS PASSED!!" << endl;
// 	return 0;
// }
