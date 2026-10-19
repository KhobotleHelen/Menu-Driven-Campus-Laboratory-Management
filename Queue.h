// Queue.h   (Student B)
// Normal request queue: First-In-First-Out (FIFO), built from a singly linked list.
// A queue keeps TWO pointers - front (where we remove) and rear (where we add) -
// so enqueue and dequeue are both O(1).
// The same class is also reused as the "processed request history" log.
// INTEGRATION CHANGES: count/size(), toArray(), and an optional "verbose" flag
// (so that loading from a file does not print a message for every request).
#ifndef QUEUE_H
#define QUEUE_H

#include "Request.h"
#include <iostream>

class RequestQueue {
private:
    // One link of the chain: a Request and a pointer to the next link.
    struct Node {
        Request data;                 // The request stored in this node.
        Node* next;                   // The node behind it (NULL at the end).
        explicit Node(const Request& request) : data(request), next(nullptr) {}
    };

    Node* frontNode;   // First request (will be served next).
    Node* rearNode;    // Last request (newest).
    int count;         // How many requests are waiting.

public:
    RequestQueue() : frontNode(nullptr), rearNode(nullptr), count(0) {}
    // Constructor: starts as an empty queue.

    ~RequestQueue();
    // Destructor: deletes every remaining node (prevents memory leaks).

    RequestQueue(const RequestQueue&) = delete;
    RequestQueue& operator=(const RequestQueue&) = delete;
    // Copying is forbidden: two queues must never share the same nodes.

    bool isEmpty() const;                                  // true when no requests are waiting
    int  size() const;                                     // number of waiting requests
    void enqueue(const Request& request, bool verbose = true);  // add at the REAR      O(1)
    bool dequeue(Request& removedRequest);                 // remove from the FRONT     O(1)
    bool peekFront(Request& frontRequest) const;           // look at the front without removing
    void display(const char* title = "Normal Requests (FIFO order)") const;  // show all, front to rear  O(n)
    Request* toArray(int& number) const;                   // copy to a new[] array (caller must delete[])
};

#endif
