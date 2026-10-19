// ResourceManager.h   (INTEGRATION - written for the combined project)
// The laboratories are stored in THREE structures at once, each used for what it does best:
//   LinkedList      : the "master" list (ordered, insert at front/end/position, display)   - Student A
//   BinarySearchTree: ordered index by ID (sorted traversals, O(log n) search)              - Student C
//   HashTable       : instant lookup by ID (average O(1))                                   - Student C
// This class keeps the three in step: every add / remove / update changes all three, so they never disagree.
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Laboratory.h"
#include "LinkedList.h"
#include "BinarySearchTree.h"
#include "HashTable.h"

// Where a new laboratory should be inserted into the linked list.
const int ADD_AT_FRONT    = 1;
const int ADD_AT_END      = 2;
const int ADD_AT_POSITION = 3;

// Result codes of addResource().
const int ADD_OK           = 0;
const int ADD_EMPTY_ID     = 1;
const int ADD_DUPLICATE    = 2;
const int ADD_BAD_POSITION = 3;

class ResourceManager {
public:
    LinkedList list;           // Master list of laboratories.
    BinarySearchTree bst;      // Index ordered by ID.
    HashTable table;           // Index for instant lookup by ID.

    int addResource(const Laboratory& lab, int where, int position = 0);
    // Adds to all three structures. Returns one of the ADD_ codes. Duplicate IDs are rejected (checked in O(1) with the hash table).

    bool removeResource(const std::string& id, Laboratory& removedCopy, int& oldPosition);
    // Removes from all three. Gives back a copy and the old list position (needed for UNDO).

    bool updateResource(const std::string& id, const Laboratory& newData);
    // Replaces the data of an existing laboratory in all three. The ID itself never changes.

    void clearAll();
    // Empties all three structures.

    int count() const;
    // Number of laboratories stored.
};

#endif
