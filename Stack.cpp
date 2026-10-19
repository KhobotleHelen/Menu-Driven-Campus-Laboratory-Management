// Stack.cpp   (Student B)
// Implementation of the linked-list undo stack.
#include "Stack.h"

// Destructor: pop everything so that no node is leaked.
UndoStack::~UndoStack() {
    UndoAction ignored;
    while (pop(ignored)) {}
}

// Empty when there is no top node.
bool UndoStack::isEmpty() const {
    return topNode == nullptr;
}

// Number of stored actions.
int UndoStack::size() const {
    return count;
}

// PUSH: place a new record on top. The new node points at the old top.
void UndoStack::push(const UndoAction& action) {
    Node* newNode = new Node(action);
    // Create a node holding the record.

    newNode->next = topNode;
    // It sits on top of the current top.

    topNode = newNode;
    // It becomes the new top.

    count++;
    // One more action stored.

    std::cout << "Action saved for undo: " << action.description << '\n';
    // Tell the user it was recorded.
}

// PUSH (text only): builds a NOTE record, then uses the main push.
void UndoStack::push(const std::string& note) {
    UndoAction action;
    action.type = "NOTE";
    action.description = note;
    push(action);
}

// POP: remove the top record and give it to the caller.
bool UndoStack::pop(UndoAction& action) {
    if (isEmpty()) {
        return false;
        // Empty-stack boundary case: nothing to pop.
    }

    Node* oldTop = topNode;
    // Remember the node to delete.

    action = oldTop->action;
    // Copy the record out.

    topNode = topNode->next;
    // The node underneath becomes the top.

    delete oldTop;
    // Free the old top.

    count--;
    // One fewer action.

    return true;
}

// POP (text only): original Student B function.
bool UndoStack::pop(std::string& description) {
    UndoAction action;
    if (!pop(action)) {
        return false;
    }
    description = action.description;
    return true;
}

// PEEK: read the top record without removing it.
bool UndoStack::peek(UndoAction& action) const {
    if (isEmpty()) {
        return false;
    }
    action = topNode->action;
    return true;
}

// PEEK (text only): original Student B function.
bool UndoStack::peek(std::string& description) const {
    if (isEmpty()) {
        return false;
    }
    description = topNode->action.description;
    return true;
}

// Print the most recent action.
void UndoStack::displayTop() const {
    if (isEmpty()) {
        std::cout << "There is no action to undo.\n";
    } else {
        std::cout << "Most recent action [" << topNode->action.type << "]: "
                  << topNode->action.description << '\n';
    }
}
