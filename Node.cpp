// This file implements the Node constructor.
#include "Node.h"
#include <cstddef>
// <cstddef> provides NULL.
// We include Node.h because we are implementing Node.

Node::Node(Laboratory lab)
{
    data = lab;
    // Store the supplied Laboratory inside this Node.

    next = NULL;
    // At first, this Node is not connected to another Node.
}

