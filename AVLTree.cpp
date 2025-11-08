#include "AVLTree.h"

#include <string>


//default constructor that sets the root pointer to null pointer
AVLTree::AVLTree()
{
    root = nullptr;
}

//class deconstructor
AVLTree::~AVLTree()
{

}


//removes a node from the tree and rebalances as necessary
bool AVLTree::removeNode(AVLNode*& current){
    if (!current) {
        return false;
    }

    AVLNode* toDelete = current;
    auto nChildren = current->numChildren();
    if (current->isLeaf()) {
        // case 1 we can delete the node
        current = nullptr;
    } else if (current->numChildren() == 1) {
        // case 2 - replace current with its only child
        if (current->right) {
            current = current->right;
        } else {
            current = current->left;
        }
    } else {
        // case 3 - we have two children,
        // get smallest key in right subtree by
        // getting right child and go left until left is null
        AVLNode* smallestInRight = current->right;
        // I could check if smallestInRight is null,
        // but it shouldn't be since the node has two children
        while (smallestInRight->left) {
            smallestInRight = smallestInRight->left;
        }
        std::string newKey = smallestInRight->key;
        int newValue = smallestInRight->value;
        remove(root, smallestInRight->key); // delete this one

        current->key = newKey;
        current->value = newValue;

        current->height = current->getHeight();
        balanceNode(current);

        return true; // we already deleted the one we needed to so return
    }
    delete toDelete;

    return true;
}

bool AVLTree::remove(AVLNode *&current, KeyType key) {
    return false;
}

void AVLTree::balanceNode(AVLNode *&node) {
}



//Node helper methods

size_t AVLTree::AVLNode::numChildren() const {
    size_t numNodes = 0;
    //checks if left node exists
    if (left != nullptr)
    {
        numNodes++;
    }
    //checks if right node exists
    if (right != nullptr)
    {
        numNodes++;
    }
    return numNodes;
}

//if both left and right node pointers are null, then node must be a leaf and returns true
bool AVLTree::AVLNode::isLeaf() const {
    if (left == nullptr && right == nullptr)
    {
        return true;
    }else
    {
        return false;
    }
}

//returns height
size_t AVLTree::AVLNode::getHeight() const {
    return height;
}
