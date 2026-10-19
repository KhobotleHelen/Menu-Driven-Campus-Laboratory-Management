// HashTable.h   (Student C)
// Hash table: Resource ID -> Laboratory, for very fast lookup by ID.
//
// HASH FUNCTION : polynomial string hash  h = (h*31 + character) mod TABLE_SIZE
// COLLISIONS    : SEPARATE CHAINING - every bucket is a linked list, so two IDs that land in the
//                 same bucket are simply stored one after the other in that bucket's list.
// WHY CHAINING  : the table can never become "full", deletion is simple (no tombstones),
//                 and performance degrades gracefully if the load factor is above 1.
// TABLE_SIZE    : a PRIME number spreads keys more evenly. (Student C used 11; it was raised to 101
//                 so that the large-data tests still keep the chains short. It is one constant to change.)
// Average lookup O(1) (chain length ~ n / TABLE_SIZE); worst case O(n) if every key collides.
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "Laboratory.h"
#include <string>
#include <iostream>

class HashTable {
private:
    static const int TABLE_SIZE = 101;
    // Number of buckets (a group parameter - change here if your lecturer assigns a different size).

    // One element of a bucket's chain.
    struct Node {
        Laboratory lab;     // The stored laboratory (a copy).
        Node* next;         // The next laboratory in the same bucket.

        Node(const Laboratory& value) {
            lab = value;
            next = nullptr;
        }
    };

    Node* table[TABLE_SIZE];   // The array of buckets (each is the head of a linked list).
    int itemCount;             // Total laboratories stored.

    int hashFunction(const std::string& id) const;
    // Converts an ID string to a bucket number 0 .. TABLE_SIZE-1.

public:
    HashTable();
    // Constructor: all buckets empty.

    ~HashTable();
    // Destructor: free all chains.

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    // Copying is forbidden.

    bool insert(const Laboratory& lab);                   // add (duplicates rejected)
    const Laboratory* search(const std::string& id) const;  // find by ID, NULL if absent
    bool remove(const std::string& id);                   // delete by ID
    int getIndex(const std::string& id) const;            // which bucket an ID maps to
    void display() const;                                 // print only non-empty buckets
    void displayStats() const;                            // items, load factor, longest chain
    int size() const;                                     // number of stored laboratories
    int tableSize() const;                                // number of buckets
    void clear();                                         // remove everything (used when reloading)
};

#endif
