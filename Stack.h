// Stack.h   (Student B)
// UNDO stack: Last-In-First-Out (LIFO), built from a singly linked list.
// All work happens at the TOP, so push, pop and peek are all O(1).
// INTEGRATION CHANGE: the original stack stored only text. It now stores an "UndoAction"
// record that remembers enough data to REALLY reverse a resource change
// (e.g. restore a deleted laboratory), as required by Module 4 of the brief.
#ifndef STACK_H
#define STACK_H

#include <string>
#include <iostream>
#include "Laboratory.h"

// Everything needed to reverse one action.
struct UndoAction {
    std::string type;          // "ADD", "REMOVE", "UPDATE" or "NOTE".
    std::string description;   // Human-readable text, e.g. "Added laboratory LAB101".
    Laboratory before;         // The laboratory BEFORE the change (used by REMOVE and UPDATE).
    Laboratory after;          // The laboratory AFTER the change (used by ADD and UPDATE).
    int position;              // Position in the linked list (used to restore a REMOVE exactly where it was).

    UndoAction() : position(0) {}
    // Default constructor: an empty record.
};

class UndoStack {
private:
    // One link of the chain.
    struct Node {
        UndoAction action;     // The record stored in this node.
        Node* next;            // The node underneath it.
        explicit Node(const UndoAction& value) : action(value), next(nullptr) {}
    };

    Node* topNode;   // The most recent action (top of the stack).
    int count;       // How many actions are stored.

public:
    UndoStack() : topNode(nullptr), count(0) {}
    // Constructor: starts empty.

    ~UndoStack();
    // Destructor: frees all nodes.

    UndoStack(const UndoStack&) = delete;
    UndoStack& operator=(const UndoStack&) = delete;
    // Copying is forbidden (two stacks must not share nodes).

    bool isEmpty() const;                          // Empty-stack detection
    int  size() const;                             // How many actions can be undone
    void push(const UndoAction& action);           // PUSH a full undo record       O(1)
    void push(const std::string& note);            // PUSH a plain text note (original Student B function)
    bool pop(UndoAction& action);                  // POP the top record            O(1)
    bool pop(std::string& description);            // POP and return only the text (original function)
    bool peek(UndoAction& action) const;           // PEEK / Top without removing   O(1)
    bool peek(std::string& description) const;     // PEEK text only (original function)
    void displayTop() const;                       // Print the most recent action
};

#endif
