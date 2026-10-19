// The Node is what allows us to turn our Laboratory objects into a Linked List.
// Nodes hold Laboratories.
#ifndef NODE_H
#define NODE_H
#include "Laboratory.h"
// Our Node needs to store a Laboratory, therefore Node.h needs to know what Laboratory is.

class Node
{
public:

    Laboratory data;
    // Each Node contains one Laboratory object.
    // This is the actual laboratory information stored in this Node.

    Node* next;
    // The next pointer allows Nodes to connect.

    Node(Laboratory lab);
    // Node constructor.
    // We create a Node by giving it a Laboratory.
};

#endif
// Ends the Node header file.

