// BinarySearchTree.cpp   (Student C)
// Implementation of the Binary Search Tree.
#include "BinarySearchTree.h"

// Constructor: an empty tree has no root.
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
    nodeCount = 0;
}

// Destructor: delete all nodes.
BinarySearchTree::~BinarySearchTree() {
    destroy(root);
}

// POST-ORDER deletion: delete the children first, then the node itself.
void BinarySearchTree::destroy(Node* node) {
    if (node == nullptr) return;
    // Base case: nothing to delete.

    destroy(node->left);
    // Delete the whole left subtree.

    destroy(node->right);
    // Delete the whole right subtree.

    delete node;
    // Finally delete this node.
}

// Delete every node and make the tree empty again.
void BinarySearchTree::clear() {
    destroy(root);
    root = nullptr;
    nodeCount = 0;
}

// RECURSIVE INSERT: returns the (possibly new) root of the subtree.
BinarySearchTree::Node* BinarySearchTree::insertNode(
    Node* node, const Laboratory& lab, bool& inserted) {

    if (node == nullptr) {
        // Found the empty spot where the new laboratory belongs.

        inserted = true;
        return new Node(lab);
    }

    if (lab.getId() < node->lab.getId()) {
        // Smaller ID: it belongs in the left subtree.

        node->left = insertNode(node->left, lab, inserted);
    }
    else if (lab.getId() > node->lab.getId()) {
        // Larger ID: it belongs in the right subtree.

        node->right = insertNode(node->right, lab, inserted);
    }
    // Equal ID = duplicate: do nothing ("inserted" stays false).

    return node;
}

// PUBLIC INSERT.
bool BinarySearchTree::insert(const Laboratory& lab) {
    if (lab.getId().empty()) return false;
    // An empty ID cannot be a key.

    bool inserted = false;
    // Will become true only if a new node is created.

    root = insertNode(root, lab, inserted);
    // Start at the root.

    if (inserted) nodeCount++;
    // Keep the counter correct.

    return inserted;
}

// SEARCH (iterative): follow left/right pointers like a binary search. O(height).
const Laboratory* BinarySearchTree::search(
    const std::string& id) const {

    Node* current = root;
    // Start at the top.

    while (current != nullptr) {
        if (id == current->lab.getId()) {
            return &current->lab;
            // Found it.
        }

        if (id < current->lab.getId()) {
            current = current->left;
            // The ID must be in the left subtree.
        }
        else {
            current = current->right;
            // The ID must be in the right subtree.
        }
    }

    return nullptr;
    // Fell off the tree: not found.
}

// The smallest ID in a subtree is found by always going left.
BinarySearchTree::Node* BinarySearchTree::findMinimum(Node* node) {
    while (node->left != nullptr) {
        node = node->left;
    }

    return node;
}

// RECURSIVE DELETE with the three classic cases.
BinarySearchTree::Node* BinarySearchTree::deleteNode(
    Node* node, const std::string& id, bool& deleted) {

    if (node == nullptr) return nullptr;
    // ID not found in this branch.

    if (id < node->lab.getId()) {
        node->left = deleteNode(node->left, id, deleted);
        // Look in the left subtree.
    }
    else if (id > node->lab.getId()) {
        node->right = deleteNode(node->right, id, deleted);
        // Look in the right subtree.
    }
    else {
        // This is the node to delete.

        deleted = true;

        if (node->left == nullptr) {
            // Case 1 and 2: no left child (leaf or only a right child).

            Node* temp = node->right;
            delete node;
            return temp;
            // The right child (or NULL) takes this node's place.
        }

        if (node->right == nullptr) {
            // Case 2: only a left child.

            Node* temp = node->left;
            delete node;
            return temp;
            // The left child takes this node's place.
        }

        // Case 3: two children. Replace this node's data with its INORDER SUCCESSOR
        // (the smallest ID in the right subtree), then delete that successor node.
        Node* successor = findMinimum(node->right);
        const std::string successorId = successor->lab.getId();
        node->lab = successor->lab;
        // Copy the successor's data into this node.

        bool removedSuccessor = false;
        node->right = deleteNode(
            node->right, successorId, removedSuccessor
        );
        // Delete the original successor node from the right subtree.
    }

    return node;
}

// PUBLIC DELETE.
bool BinarySearchTree::remove(const std::string& id) {
    bool deleted = false;
    root = deleteNode(root, id, deleted);
    if (deleted) nodeCount--;
    return deleted;
}

// INORDER: left subtree, this node, right subtree  -> ascending ID order.
void BinarySearchTree::inorderNode(Node* node) const {
    if (node == nullptr) return;

    inorderNode(node->left);
    std::cout << node->lab.getId() << " ";
    inorderNode(node->right);
}

// PREORDER: this node, left subtree, right subtree.
void BinarySearchTree::preorderNode(Node* node) const {
    if (node == nullptr) return;

    std::cout << node->lab.getId() << " ";
    preorderNode(node->left);
    preorderNode(node->right);
}

// POSTORDER: left subtree, right subtree, this node.
void BinarySearchTree::postorderNode(Node* node) const {
    if (node == nullptr) return;

    postorderNode(node->left);
    postorderNode(node->right);
    std::cout << node->lab.getId() << " ";
}

void BinarySearchTree::inorder() const {
    inorderNode(root);
    std::cout << '\n';
}

void BinarySearchTree::preorder() const {
    preorderNode(root);
    std::cout << '\n';
}

void BinarySearchTree::postorder() const {
    postorderNode(root);
    std::cout << '\n';
}

// HEIGHT: 1 + the taller of the two subtrees. A balanced tree has height ~ log2(n); a chain has height n.
int BinarySearchTree::heightNode(Node* node) const {
    if (node == nullptr) return 0;
    int leftHeight = heightNode(node->left);
    int rightHeight = heightNode(node->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int BinarySearchTree::height() const {
    return heightNode(root);
}

int BinarySearchTree::size() const {
    return nodeCount;
}

bool BinarySearchTree::isEmpty() const {
    return root == nullptr;
}
