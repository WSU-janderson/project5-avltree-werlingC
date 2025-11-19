#include "AVLTree.h"

#include <string>


//default constructor that sets the root pointer to null pointer
AVLTree::AVLTree()
{
    root = nullptr;
    treeSize = 0;
}

//copy constructor that takes another tree and copys all value into tree on left hand side
AVLTree::AVLTree(const AVLTree& otherTree)
{
    root = copy(otherTree.root);
    treeSize = otherTree.treeSize;
}

//Inserts a new node. Starts the insert process and calls a recursive method
bool AVLTree::insert(const std::string& key, size_t value)
{
    //variable that stores whether or not the new node was able to be inserted
    bool success = insertRecursive(root, key, value);
    if (success)
    {
        //increase size of tree on success
        treeSize++;
    }
    return success;
}

//Public call for the remove method.
bool AVLTree::remove(const KeyType& key)
{
    //variable that holds whether or not the node was able to be removed
    bool success = remove(root, key);
    if (success)
    {
        //decrease size of tree on success
        treeSize--;
    }
    return success;
}


//calls the recursive contains method that searches the tree for a given key
bool AVLTree::contains(const std::string& key) const
{
    return containsRecursive(root, key);
}

//public call for the get mehtod that calls the recursive helper method
optional<size_t> AVLTree::get(const std::string& key) const
{
    return getRecursive(root, key);
}

//public call for the bracket operator override. Calls a recursive method to find the value associated with the given key
size_t& AVLTree::operator[](const std::string& key)
{
    return (bracketRecursive(root, key)).value;
}

//takes in two keys and calls a recursive method to find all keys in between them
vector<size_t> AVLTree::findRange(const std::string& lowKey, const std::string& highKey) const
{
    //vector that holds the result
    vector<size_t> result;

    findRangeRecursive(root, lowKey, highKey, result);
    return result;
}

//Returns all keys in the tree as a vector of string in order
std::vector<std::string> AVLTree::keys() const
{
    //Creates a vector, then calls the recursive method to gather all keys in order from the tree
    vector<string> keyVector;
    keysRecursive(root, keyVector);
    return keyVector;
}

//recursive helper methods that will recusrively call itself to locate the node that has the given key
optional<size_t> AVLTree::getRecursive(AVLNode* node, const KeyType& key) const
{
    //null node means end of tree, which means the key was not found
    if (node == nullptr)
    {
        return nullopt;
    }
    //key was found and returns value
    if (key == node->key)
    {
        return node->value;
    }

    //Key not found yet, decides which branch to follow to continue searching
    if (key < node->key)
    {
        return getRecursive(node->left, key);
    }else
    {
        return getRecursive(node->right, key);
    }
}

//Recursive helper method for the findRange method
void AVLTree::findRangeRecursive(AVLNode* node, const KeyType& lowKey, const KeyType& highKey, vector<size_t>& result) const
{
    //checks that node is not null
    if (node == nullptr)
    {
        return;
    }

    //checks which why to traverse the tree
    //goes to the left branch
    if (node->key > lowKey)
    {
        findRangeRecursive(node->left, lowKey, highKey, result);
    }
    //found a node we are looking for
    if (node->key >= lowKey && node->key <= highKey)
    {
        result.push_back(node->value);
    }
    //goes to the right branch
    if (node->key < highKey)
    {
        findRangeRecursive(node->right, lowKey, highKey, result);
    }
}

//Recursive helper method for the bracket operator
AVLTree::AVLNode& AVLTree::bracketRecursive(AVLNode*& node, const std::string& key)
{
    //checks for null node
    if (node == nullptr)
    {
        return *node;
    }

    //goes left
    if (key < node->key)
    {
        return bracketRecursive(node->left, key);
    }
    //goes right
    else if (key > node->key)
    {
        return bracketRecursive(node->right, key);
    }
    //node found
    else
    {
        return *node;
    }


}

//recursive helper to release all nodes from memory
void AVLTree::clear(AVLNode*& node)
{
    //goes through the tree with in-order traversal
    if (node!= nullptr)
    {
        //clears left and right subtrees recursively before deleting the root node
        clear(node->left);
        clear(node->right);
        delete node;
        node = nullptr;
    }
}

//recursive copy method that copys all nodes from a start node down are copied and linked together, and ultimatly returning the root
AVLTree::AVLNode* AVLTree::copy(const AVLNode* node) const
{
    //checks for null node (end of tree)
    if (node == nullptr)
    {
        return nullptr;
    }

    //creates a new node and copies the data
    AVLNode* newNode = new AVLNode();
    newNode->key = node->key;
    newNode->value = node->value;
    newNode->height = node->height;

    //Calls copy for children recursively
    newNode->left = copy(node->left);
    newNode->right = copy(node->right);

    //returns the node
    return newNode;
}

//recursive helper for the keys function to search the tree for all keys
void AVLTree::keysRecursive(AVLNode* node, vector<string>& keyVector) const
{
    //checks for null node (end of tree)
    if (node == nullptr)
    {
        return;
    }

    //recursive call for the keysRecursive method
    keysRecursive(node->left, keyVector);
    //adds current node before right subtree nodes to maintain in-order traversal
    keyVector.push_back(node->key);
    keysRecursive(node->right, keyVector);
}

//recursive method for contains the searches a tree branch for where they key should be located
bool AVLTree::containsRecursive(AVLNode* node, const string& key) const
{
    //if the node is null, then end of tree has been reached and key is not in tree
    if (node == nullptr)
    {
        return false;
    }
    //key found
    if (key == node->key)
    {
        return true;
    }

    //if node does not contain key, decide whether to navigate left or right to find it
    if (key < node->key)
    {
        return containsRecursive(node->left, key);
    }else
    {
        return containsRecursive(node->right, key);
    }
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

//= operator override that allows for a copy of a tree to be placed into another tree object
void AVLTree::operator=(const AVLTree& otherTree)
{
    //checks for self-assignment
    if (this == &otherTree)
    {
        return;
    }

    //empties the current tree so that it can be overwritten
    clear(root);

    //copys all nodes from the other tree
    root = copy(otherTree.root);
    treeSize = otherTree.treeSize;
}

//class deconstructor
AVLTree::~AVLTree()
{
    //calls the clear method, which recursively frees up the memory that each node in the tree took up.
    clear(root);
}

//recursive helper method the finds the correct location to place the new node
bool AVLTree::insertRecursive(AVLNode*& node, const KeyType& key, ValueType value)
{
    //Case where the correct spot is found
    if (node == nullptr)
    {
        //inserts information into a new node
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
    //goes to the left
    if (key < node->key)
    {
        //if the key is less than current node's key, search it's left branch
        success = insertRecursive(node->left, key, value);
    }
    //goes right
    else if (key > node->key)
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
        //height is the larger value between its two branches
        node->height = (1 + max(getHeight(node->left), getHeight(node->right)));
    }
}

//Calculates a node's balance factor
int AVLTree::getBalance(AVLNode* node) const
{
    if (node == nullptr)
    {
        //null nodes are height -1 since leaf nodes are 0. Also serves to let the program know it's reached the end of the tree
        return -1;
    }else
    {
        //Gets height of the left and right subtrees and subtracts them, then returns that value
        return (getHeight(node->left) - getHeight(node->right));
    }
}


//rotation and balance functions

//performs a right rotation on a subtree based on a given node
void AVLTree::rotateRight(AVLNode*& node)
{
    //gets nodes involved in the rotation
    AVLNode* pivot = node->left;
    AVLNode* hook = pivot->right;

    //rotates right
    pivot->right = node;
    node->left = hook;

    //updates height of old and new root
    updateHeight(node);
    updateHeight(pivot);

    //the root node is set to what was the pivot
    node = pivot;
}

//performs a left rotation on a subtree based on a given node
void AVLTree::rotateLeft(AVLNode*& node)
{
    AVLNode* pivot = node->right;
    AVLNode* hook = pivot->left;

    //rotate left
    pivot->left = node;
    node->right = hook;

    //updates old root's height
    updateHeight(node);
    //updates new root's height
    updateHeight(pivot);

    //update pointer to new root
    node = pivot;
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

//recursive helper method that finds the node with the key that is to be deleted
bool AVLTree::remove(AVLNode *&current, KeyType key) {
    //checks for null node
    if (current == nullptr)
    {
        return false;
    }

    //variable that determines if value was found
    bool success = false;

    //goes left
    if (key < current->key)
    {
        success = remove(current->left, key);
    }
    //goes right
    else if (key > current->key)
    {
        success = remove(current->right, key);
    }
    //node found
    else
    {
        success = removeNode(current);
    }

    //rebalances the node if the key was removed
    if (success)
    {
        balanceNode(current);
    }

    return success;

}

//takes the given node and determines if it needs
void AVLTree::balanceNode(AVLNode *&node) {
    if (node == nullptr)
    {
        return;
    }

    //updates height
    updateHeight(node);

    //balance factor should be between -1 and 1
    int balanceFactor = getBalance(node);

    //Right rotation needed, leftside heavy
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

    //Right rotation needed, rightside heavy
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

//returns the number of children a node has
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

//returns height of a node
size_t AVLTree::AVLNode::getHeight() const {
    return height;
}

//ostream methods

//recursive method that puts all key-value pairs into an os stream object.
//Uses in-order traversal
void AVLTree::printTree(ostream& os, AVLNode* node) const
{
    //reached the end of the tree
    if (node == nullptr)
    {
        return;
    }

    //In-Order traversal. Recursively calls the left branch and adds those key-value pairs to the ostream.
    //Then the current node is added to ostream, then the right branch is added to the ostream
    printTree(os, node->left);
    //I had to put everything into a string since for some reason the ostream was not letting me chain << operators.
    string outStream = "{" + node->key + ": " + to_string(node->value) + "}\n";
    os << outStream;
    printTree(os, node->right);
}

//Overrides the << operator to allow for the whole tree to be output
ostream& operator<<(std::ostream& os, const AVLTree& avlTree)
{
    //calls the recursive helper to print the tree
    avlTree.printTree(os, avlTree.root);
    return os;
}
