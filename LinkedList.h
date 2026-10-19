// This file describes our custom Linked List.
// The Linked List dynamically stores many Laboratory objects.
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"
// A Linked List is made from Nodes.
// Therefore LinkedList needs to know what a Node is.

class LinkedList
{
private:

    Node* head;
    // head points to the FIRST Node in the list.
    int count;
    // Stores the number of laboratories currently in the list.

public:

    LinkedList();
    // Constructor.
    // Creates an empty Linked List.

    LinkedList(const LinkedList& other);
    // Copy constructor.
    // Creates a completely separate copy of another Linked List.

    ~LinkedList();
    // Destructor.
    // Deletes all dynamically created Nodes when the list is destroyed.

    LinkedList& operator=(const LinkedList& other);
    // Assignment operator.
    // Allows one Linked List to be assigned to another safely.

    bool isEmpty() const;
    // Returns true if the list contains no laboratories.

    int size() const;
    // Returns the number of laboratories in the list.

    void addFront(Laboratory lab);
    // Adds a laboratory at the beginning.

    void addEnd(Laboratory lab);
    // Adds a laboratory at the end.

    bool addAtPosition(Laboratory lab, int position);
    // Adds a laboratory at a specified position (1 = first).
    // INTEGRATION CHANGE: now returns true if inserted, false if the position does not exist.

    void display() const;
    // Displays all laboratories in the list.

    bool removeById(string id);
    // Removes a laboratory using its ID.

    bool updateById(string id, Laboratory updatedLab);
    // Replaces a laboratory with updated information.

    Laboratory* searchById(string id);
    // Searches for a laboratory by ID.
    // Returns a pointer to the Laboratory if found.
    // Returns NULL if not found.

    int positionOfId(string id) const;
    // INTEGRATION ADDITION: returns the 1-based position of the laboratory with this ID,
    // or 0 if it is not in the list. Used by UNDO to restore a deleted laboratory to its old position.

    void clear();
    // Removes every Node from the list.

    Laboratory* toArray(int& number) const;
    // Creates a dynamic array containing copies of the laboratories.
    // This will help us perform binary searching and sorting.
};

#endif

