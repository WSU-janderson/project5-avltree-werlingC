/**
 * AVLTree.h
 */

#ifndef AVLTREE_H
#define AVLTREE_H
#include <optional>
#include <string>
#include <vector>

using namespace std;

class AVLTree {
public:
    using KeyType = std::string;
    using ValueType = size_t;

    /**
     *default constructor
     */
    AVLTree();

    /**
     *copy constructor
     */
    AVLTree(const AVLTree& other);

    /**
    *Inserts a new key-value pair into the tree. Rebalnces the tree as necessary.
    */
    bool insert(const std::string& key, size_t value);

    /**
    *Removes a node from a tree if the key is in the tree. Rebalances after removal if necessary.
    */
    bool remove(const KeyType& key);


    /**
    *Returns true if the tree does contain the method, and false if it does not.
    */
    bool contains(const std::string& key) const;

    /**
    *If the key is in the tree, then get will return the value assocaited with it.
    */
    optional<size_t> get(const std::string& key) const;


    /**
    *[] operator override that allows for individual values in the tree to be returned as a reference
    */
    size_t& operator[](const std::string& key);

    /**
    *Returns a vector that returns all keys between two ranges.
    */
    vector<size_t> findRange(const std::string& lowKey, const std::string& highKey) const;


    /**
    *Returns a vector that contains all keys currently in the tree
    */
    std::vector<std::string> keys() const;

    /**
    *returns the number of key value pairs in the tree
    */
    size_t size() const;

    /**
    *Returns the height of the AVL tree
    */
    size_t getHeight() const;


    /**.
    *= operator overload. Creates a deep copy of another tree and puts it into the tree that called it.
    * Deletes memory that is no longer being used after the assignment.
    */
    void operator=(const AVLTree& other);

    /**
    *Destructor for the AVLTree class
    */
    ~AVLTree();





protected:
    class AVLNode {
    public:
        KeyType key;
        ValueType value;
        size_t height;

        AVLNode* left;
        AVLNode* right;

        // 0, 1 or 2
        size_t numChildren() const;
        // true or false
        bool isLeaf() const;
        // number of hops to deepest leaf node
        size_t getHeight() const;


    };

public:




    private:
    AVLNode* root;
    size_t treeSize;

    //insert helper method
    bool insertRecursive(AVLNode*& node, const KeyType& key, ValueType value);


    //Tree balancing methods
    /**
     *helper method that gets the height of a node and handles null pointers
     */
    static int getHeight(AVLNode* node);

    /**
     *Recursive helper method that updates a node's height based on chlildren's heights
     */
    void updateHeight(AVLNode*);

    /**
     *Calculates the balance factor of a node
     */
    int getBalance(AVLNode* node) const;

    /**
     *performs a right rotations on a subtree.
     *The given node acts as the root of the subtree
     */
    void rotateRight(AVLNode*& node);

    /**
     *performs a left rotation on a subtree
     *The given node acts as the root of the subtree
     */
    void rotateLeft(AVLNode*& node);


    /* Helper methods for remove */
    // this overloaded remove will do the recursion to remove the node
    bool remove(AVLNode*& current, KeyType key);
    // removeNode contains the logic for actually removing a node based on the numebr of children
    bool removeNode(AVLNode*& current);
    // You will implement this, but it is needed for removeNode()
    void balanceNode(AVLNode*& node);


    bool containsRecursive(AVLNode* node, const string& key) const;

    /**
         *Recursive helper method for get that will search the tree recursively for a key and return the associated value if found.
         */
    optional<size_t> getRecursive(AVLNode* node, const KeyType& key) const;

    /**
    *Recursive findRange helper method the performs in-order traversal to find all keys
     */
    void findRangeRecursive(AVLNode* node, const KeyType& lowKey, const KeyType& highKey, vector<size_t>& result) const;

    /**
     *Recursive helper method that returns a reference to a value in a node
     */
    AVLNode& bracketRecursive(AVLNode*& node, const std::string& key);

    /**
     *recursive method to clear a tree upon deletion and release memeory
     */
    void clear(AVLNode*& node);

    /**
     *Recursive helper for copying data from one tree to another
     */
    AVLNode* copy(const AVLNode* node) const;

    /**
    *recursive method to get all data key value pairs in a tree and appends them to an os object to be output
    */
    void printTree(ostream& os, AVLNode* node) const;

    /**
    *Outputs all nodes in the AVL tree in the format "{Key: value}"
    */
    friend std::ostream& operator<<(ostream& os, const AVLTree & avlTree);
};

#endif //AVLTREE_H
