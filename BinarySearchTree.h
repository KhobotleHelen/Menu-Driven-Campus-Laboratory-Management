// BinarySearchTree.h   (Student C)
// Binary Search Tree (BST) of laboratories, ordered by laboratory ID (the tree key).
// Rule: every ID in the LEFT subtree is smaller than the node, every ID in the RIGHT subtree is larger.
// Average search/insert/delete: O(log n).  Worst case (a chain, e.g. IDs inserted in sorted order): O(n).
// INTEGRATION CHANGES: uses Student A's Laboratory class (getId() instead of .id),
// plus clear(), height() and size() helpers.
#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "Laboratory.h"
#include <string>
#include <iostream>

class BinarySearchTree {
private:
    // One tree node: a laboratory and two child pointers.
    struct Node {
        Laboratory lab;     // The laboratory stored here (a copy).
        Node* left;         // Child with a SMALLER ID.
        Node* right;        // Child with a LARGER ID.

        Node(const Laboratory& value) {
            lab = value;
            left = nullptr;
            right = nullptr;
            // A new node starts as a leaf (no children).
        }
    };

    Node* root;     // Top of the tree (NULL when the tree is empty).
    int nodeCount;  // How many laboratories are stored.

    // Recursive helper functions (the public functions call these starting at the root).
    Node* insertNode(Node* node, const Laboratory& lab, bool& inserted);
    Node* deleteNode(Node* node, const std::string& id, bool& deleted);
    Node* findMinimum(Node* node);
    void inorderNode(Node* node) const;
    void preorderNode(Node* node) const;
    void postorderNode(Node* node) const;
    int heightNode(Node* node) const;
    void destroy(Node* node);

public:
    BinarySearchTree();
    // Constructor: empty tree.

    ~BinarySearchTree();
    // Destructor: deletes every node.

    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;
    // Copying is forbidden (would share nodes).

    bool insert(const Laboratory& lab);               // add (duplicates are rejected)
    const Laboratory* search(const std::string& id) const;  // find by ID, NULL if absent
    bool remove(const std::string& id);               // delete (handles 0, 1 and 2 children)

    void inorder() const;     // Left, Node, Right  -> IDs in SORTED order
    void preorder() const;    // Node, Left, Right  -> useful for copying a tree
    void postorder() const;   // Left, Right, Node  -> useful for deleting a tree

    int  height() const;      // Number of levels (0 for an empty tree)
    int  size() const;        // Number of laboratories stored
    bool isEmpty() const;     // true when the tree has no nodes
    void clear();             // delete everything (used when reloading data)
};

#endif
