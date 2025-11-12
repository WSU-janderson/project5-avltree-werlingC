#include "AVLTree.h"

#include <string>


//default constructor that sets the root pointer to null pointer
AVLTree::AVLTree()
{
    root = nullptr;
    treeSize = 0;
}

//Inserts a new node. Starts the insert process and calls a recursive method
bool AVLTree::insert(const std::string& key, size_t value)
{
    bool success = insertRecursive(root, key, value);
    if (success)
    {
        treeSize++;
    }
    return success;
}

//returns how many nodes are in the AVL tree
size_t AVLTree::size() const
{
    return treeSize;
}

//returns the current height of the tree
size_t AVLTree::getHeight() const
{
    return getHeight(root);
}

//class deconstructor
AVLTree::~AVLTree()
{

}

//recursive helper method the finds the correct location to place the new node
bool AVLTree::insertRecursive(AVLNode*& node, const KeyType& key, ValueType value)
{
    //Case where the correct spot is found
    if (node == nullptr)
    {
        node = new AVLNode();
        node->key = key;
        node->value = value;
        node->left = nullptr;
        node->right = nullptr;
        node->height = 0;
        return true;
    }

    //marks success as false because the current node can not be inserted on
    bool success = false;

    //Continue searching down the tree
    if (key < node->key)
    {
        //if the key is less than current node's key, search it's left branch
        success = insertRecursive(node->left, key, value);
    }else if (key > node->key)
    {
        //if the key is greater than the current node's key, search it's right tree
        success = insertRecursive(node->right, key, value);
    }else
    {
        //duplicate key found
        success = false;
    }

    //calls for the balance function to ensure the tree has not been unbalanced due to the new insertion
    if (success)
    {
        balanceNode(node);
    }

    return success;
}

//gets the height of a node
int AVLTree::getHeight(AVLNode* node)
{
    if (node == nullptr)
    {
        return 0;
    }else
    {
        return node->height;
    }
}

//Updates the node height by getting the height from both of its children and taking the larger height value
void AVLTree::updateHeight (AVLNode* node)
{
    if (node != nullptr)
    {
        node->height = (1 + std::max(getHeight(node->left), getHeight(node->right)));
    }
}

//Calculates a node's balance factor
int AVLTree::getBalance(AVLNode* node) const
{
    if (node == nullptr)
    {
        return -1;
    }else
    {
        return (getHeight(node->left) - getHeight(node->right));
    }
}


//rotation and balance functions

//performs a right rotation on a subtree based on a given node
void AVLTree::rotateRight(AVLNode*& node)
{
    //gets nodes involved in the rotation
    AVLNode* nodeA = node->left;
    AVLNode* nodeB = nodeA->right;

    //rotate right
    nodeA->right = node;
    node->left = nodeB;

    //updates height of old and new root
    updateHeight(node);
    updateHeight(nodeA);

    node = nodeA;
}

//performs a left rotation on a subtree based on a given node
void AVLTree::rotateLeft(AVLNode*& node)
{
    AVLNode* nodeA = node->right;
    AVLNode* nodeB = nodeA->left;

    //rotate left
    nodeA->left = node;
    node->right = nodeB;

    //updates old root's height
    updateHeight(node);
    //updates new root's height
    updateHeight(nodeA);

    //update pointer to new root
    node = nodeA;
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
    if (node == nullptr)
    {
        return;
    }

    //updates height
    updateHeight(node);

    int balanceFactor = getBalance(node);

    //Right rotation needed
    if (balanceFactor > 1 && getBalance(node->left) >= 0)
    {
        rotateRight(node);
    }

    //Left-Right rotation needed
    else if (balanceFactor > 1 && getBalance(node->left) < 0)
    {
        rotateLeft(node->left);
        rotateRight(node);
    }

    //Right rotation needed
    else if (balanceFactor < -1 && getBalance(node->right) <= 0)
    {
        rotateRight(node);
    }
    //Right-left rotation needed
    else if (balanceFactor < -1 && getBalance(node->right) > 0)
    {
        rotateRight(node->right);
        rotateLeft(node);
    }
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

//ostream methods

//recursive method that puts all key-value pairs into an os stream object.
//Uses in-order traversal
void AVLTree::printTree(ostream& os, AVLNode* node) const
{
    if (node == nullptr)
    {
        return;
    }
    //In-Order traversal
    printTree(os, node->left);
    string outStream = "{" + node->key + ": " + to_string(node->value) + "}\n";
    os << outStream;
    printTree(os, node->right);
}

//Overrides the << operator to allow for the whole tree to be output
std::ostream& operator<<(std::ostream& os, const AVLTree& avlTree)
{
    avlTree.printTree(os, avlTree.root);
    return os;
}
