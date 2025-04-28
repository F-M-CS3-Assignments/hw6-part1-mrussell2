
#include <iostream>
#include <string>
#include <cassert>
#include <random>
#include "RedBlackTree.h"

using namespace std;

//used ZyBook chapter 16 as a reference on Red-Black trees
//another reference used: https://www.geeksforgeeks.org/red-black-tree-in-cpp/


//default constructor, initializes empty tree
RedBlackTree::RedBlackTree() {
    root = nullptr;
    numItems = 0;
}
//constructor with int input, insert newData
RedBlackTree::RedBlackTree(int newData) {
    Insert(newData);
}
//copy constructor, copies another red black tree
RedBlackTree::RedBlackTree(const RedBlackTree& other) {
    root = CopyOf(other.root);
    numItems = other.numItems;
}
//Returns string representation in infix order.
string RedBlackTree::ToInfixString(const RBTNode* node) {
    if (node == nullptr) return "";
    return ToInfixString(node->left) +
           " " + (node->color == COLOR_RED ? "R" : "B") + to_string(node->data) + " " +
           ToInfixString(node->right);
}
//Returns string representation in prefix order.
string RedBlackTree::ToPrefixString(const RBTNode* node) {
    if (node == nullptr) return "";
    return std::string(" ") +
       (node->color == COLOR_RED ? "R" : "B") + to_string(node->data) + " " +
       ToPrefixString(node->left) +
       ToPrefixString(node->right);
}
// Returns string representation in postfix order.
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
    node->color = COLOR_RED; // always insert new nodes red first (standard Red-Black property)

    if (root == nullptr) { // if the root is null set color to black and sent root to input node
        node->color = COLOR_BLACK;
        root = node;
        numItems++;
        return;
    }
    BasicInsert(node); // if root is not empty use BasicInsert to insert node

    // If parent is red, violation of Red-Black property occurs — call InsertFixUp
    if (node->parent != nullptr && node->parent->color == COLOR_RED) {
        InsertFixUp(node);
    }
    numItems++;
}
//function to check if data is in the tree
bool RedBlackTree::Contains(int data) const {
    return Get(data) != nullptr;
}

//finds a node with specific value or returns nullptr
RBTNode* RedBlackTree::Get(int data) const {
    RBTNode* curr = root;
    while (curr) {
        if (curr->data == data) return curr;
        curr = (data < curr->data) ? curr->left : curr->right;
    }
    return nullptr;
}

//finds the uncle of a node
RBTNode* RedBlackTree::GetUncle(RBTNode* node) const {
    if (!node || !node->parent || !node->parent->parent) return nullptr;
    RBTNode* gp = node->parent->parent;
    return (gp->left == node->parent) ? gp->right : gp->left;
}

//returns true if node is left child of parent
bool RedBlackTree::IsLeftChild(RBTNode* node) const {
    return node && node->parent && node->parent->left == node;
}
//returns true if node is right child of a parent
bool RedBlackTree::IsRightChild(RBTNode* node) const {
    return node && node->parent && node->parent->right == node;
}
// returns minimum value in tree
int RedBlackTree::GetMin() const {
    if (!root) throw runtime_error("Tree is empty");
    RBTNode* node = root;
    while (node->left) node = node->left;
    return node->data;
}

//returns maximum value in tree
int RedBlackTree::GetMax() const {
    if (!root) throw runtime_error("Tree is empty");
    RBTNode* node = root;
    while (node->right) node = node->right;
    return node->data;
}

//restores Red-Black tree properties after insertion
//handles color changes and rotations based on tree after insertion
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

//standard binary search tree insertion 
void RedBlackTree::BasicInsert(RBTNode* node) {
    RBTNode* current = root;
    RBTNode* parent = nullptr;

    //traverse tree to find correct position for node
    while (current != nullptr) {
        parent = current;
        if (node->data < current->data)
            current = current->left;
        else
            current = current->right;
    }

    //link node to parrent node
    node->parent = parent;
    if (node->data < parent->data)
        parent->left = node;
    else
        parent->right = node;
}

//copies the subtree at rooted node
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

//rotates subtree left around given node n
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
        root = rChild; // rotation at root
    }

    rChild->parent = n->parent;
    rChild->left = n;
    n->parent = rChild;
    n->right = rlGranChild;
    if (rlGranChild)
        rlGranChild->parent = n;
}
//rotates subtree right around given node n
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
        root = lChild; // rotation at root
    }

    lChild->parent = n->parent;
    lChild->right = n;
    n->parent = lChild;
    n->left = lrGranChild;
    if (lrGranChild)
        lrGranChild->parent = n;
}
