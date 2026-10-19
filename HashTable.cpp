// HashTable.cpp   (Student C)
// Implementation of the separate-chaining hash table.
#include "HashTable.h"

// Constructor: every bucket starts empty (NULL).
HashTable::HashTable() {
    itemCount = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }
}

// Destructor: delete every chain.
HashTable::~HashTable() {
    clear();
}

// Free all nodes and make all buckets empty.
void HashTable::clear() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        // Start at the head of bucket i.

        while (current != nullptr) {
            Node* nextNode = current->next;
            // Save the next node before deleting this one.

            delete current;
            current = nextNode;
        }
        table[i] = nullptr;
    }
    itemCount = 0;
}

// HASH FUNCTION: mix every character into a running number, keeping it below TABLE_SIZE.
int HashTable::hashFunction(const std::string& id) const {
    unsigned int hash = 0;
    // Running hash value.

    for (std::size_t i = 0; i < id.size(); i++) {
        // Visit each character of the ID.

        hash = (hash * 31 + static_cast<unsigned char>(id[i]))
               % TABLE_SIZE;
        // Multiply by 31 (a small prime), add the character code, wrap around the table size.
    }

    return static_cast<int>(hash);
}

// Public wrapper so tests and menus can show the bucket of an ID.
int HashTable::getIndex(const std::string& id) const {
    return hashFunction(id);
}

// INSERT: compute the bucket, reject duplicates, add at the front of the chain (O(1) after hashing).
bool HashTable::insert(const Laboratory& lab) {
    if (lab.getId().empty()) {
        return false;
        // An empty ID cannot be a key.
    }

    int index = hashFunction(lab.getId());
    // Which bucket?

    Node* current = table[index];
    // Walk the chain to look for a duplicate ID.

    while (current != nullptr) {
        if (current->lab.getId() == lab.getId()) {
            return false;
            // Duplicate ID rejected.
        }

        current = current->next;
    }

    Node* newNode = new Node(lab);
    // Create the node.

    newNode->next = table[index];
    // It points at the old head of the chain...

    table[index] = newNode;
    // ...and becomes the new head (this is how a COLLISION is handled).

    itemCount++;
    return true;
}

// SEARCH: hash the ID, then scan only that bucket's chain.
const Laboratory* HashTable::search(const std::string& id) const {
    if (id.empty()) {
        return nullptr;
    }

    int index = hashFunction(id);
    // Jump straight to the right bucket.

    Node* current = table[index];
    while (current != nullptr) {
        if (current->lab.getId() == id) {
            return &current->lab;
            // Found.
        }

        current = current->next;
    }

    return nullptr;
    // Not in this bucket, therefore not in the table.
}

// REMOVE: unlink the node from its chain.
bool HashTable::remove(const std::string& id) {
    if (id.empty()) {
        return false;
    }

    int index = hashFunction(id);

    Node* current = table[index];
    Node* previous = nullptr;
    // "previous" trails behind "current" so we can unlink.

    while (current != nullptr) {
        if (current->lab.getId() == id) {
            if (previous == nullptr) {
                table[index] = current->next;
                // Removing the head of the chain.
            }
            else {
                previous->next = current->next;
                // Removing from the middle/end: skip over it.
            }

            delete current;
            itemCount--;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
    // Nothing with that ID.
}

// DISPLAY: show each non-empty bucket and the IDs chained in it.
void HashTable::display() const {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table[i] == nullptr) continue;
        // Skip empty buckets to keep the output short.

        std::cout << "Bucket " << i << ": ";

        Node* current = table[i];

        while (current != nullptr) {
            std::cout << current->lab.getId() << " -> ";
            current = current->next;
        }

        std::cout << "NULL\n";
    }
}

// STATISTICS: helps justify the table size and the collision strategy.
void HashTable::displayStats() const {
    int usedBuckets = 0;
    // Buckets that contain at least one item.

    int longestChain = 0;
    // Length of the longest chain (worst-case lookup cost).

    for (int i = 0; i < TABLE_SIZE; i++) {
        int length = 0;
        Node* current = table[i];
        while (current != nullptr) {
            length++;
            current = current->next;
        }
        if (length > 0) usedBuckets++;
        if (length > longestChain) longestChain = length;
    }

    std::cout << "Stored items       : " << itemCount << '\n';
    std::cout << "Number of buckets  : " << TABLE_SIZE << '\n';
    std::cout << "Buckets in use     : " << usedBuckets << '\n';
    std::cout << "Load factor        : " << static_cast<double>(itemCount) / TABLE_SIZE << '\n';
    std::cout << "Longest chain      : " << longestChain << '\n';
}

int HashTable::size() const {
    return itemCount;
}

int HashTable::tableSize() const {
    return TABLE_SIZE;
}
