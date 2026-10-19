// ResourceManager.cpp   (INTEGRATION)
#include "ResourceManager.h"

// ADD: validate, then insert into the list, the BST and the hash table.
int ResourceManager::addResource(const Laboratory& lab, int where, int position)
{
    if (lab.getId().empty()) {
        return ADD_EMPTY_ID;
        // An empty ID cannot be used as a key.
    }

    if (table.search(lab.getId()) != NULL) {
        return ADD_DUPLICATE;
        // The hash table tells us in O(1) that the ID already exists.
    }

    if (where == ADD_AT_POSITION && (position < 1 || position > list.size() + 1)) {
        return ADD_BAD_POSITION;
        // Valid positions are 1 .. size+1 (size+1 means "at the end").
    }

    if (where == ADD_AT_FRONT) {
        list.addFront(lab);
        // Linked list: insert at the beginning, O(1).
    } else if (where == ADD_AT_POSITION) {
        list.addAtPosition(lab, position);
        // Linked list: insert at a chosen position, O(n).
    } else {
        list.addEnd(lab);
        // Linked list: insert at the end.
    }

    bst.insert(lab);
    // Add to the tree index.

    table.insert(lab);
    // Add to the hash index.

    return ADD_OK;
}

// REMOVE from all three structures.
bool ResourceManager::removeResource(const std::string& id, Laboratory& removedCopy, int& oldPosition)
{
    const Laboratory* found = table.search(id);
    // O(1) existence check.

    if (found == NULL) {
        return false;
        // Nothing to remove.
    }

    removedCopy = *found;
    // Keep a copy BEFORE it is destroyed (UNDO needs it).

    oldPosition = list.positionOfId(id);
    // Remember where it was in the list.

    list.removeById(id);
    bst.remove(id);
    table.remove(id);
    // Delete it everywhere.

    return true;
}

// UPDATE: replace the data in all three structures.
bool ResourceManager::updateResource(const std::string& id, const Laboratory& newData)
{
    if (table.search(id) == NULL) {
        return false;
        // Cannot update something that does not exist.
    }

    Laboratory fixed = newData;
    fixed.setId(id);
    // The ID is the key of every structure, so it must stay the same.

    list.updateById(id, fixed);
    // Replace in the linked list.

    bst.remove(id);
    bst.insert(fixed);
    // Re-insert in the BST (the stored copy changed).

    table.remove(id);
    table.insert(fixed);
    // Re-insert in the hash table.

    return true;
}

// Empty everything (used before reloading from files).
void ResourceManager::clearAll()
{
    list.clear();
    bst.clear();
    table.clear();
}

int ResourceManager::count() const
{
    return list.size();
}
