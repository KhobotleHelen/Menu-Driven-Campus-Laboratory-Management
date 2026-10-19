// PriorityQueue.h   (Student B)
// Priority request queue. Implemented as a linked list that is kept SORTED by priority:
//   1 = Emergency, 2 = High, 3 = Normal, 4 = Low   (smaller number = more urgent).
// The most urgent request is always at the head, so removing it is O(1).
// Inserting needs to find the right place, so it is O(n) in the worst case.
// Requests with EQUAL priority keep their arrival order (FIFO among equals).
// (A binary heap would make insertion O(log n); see the report for the comparison.)
// INTEGRATION CHANGES: size(), toArray() and the optional "verbose" flag.
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "Request.h"
#include <iostream>

class PriorityRequestQueue {
private:
    // One link of the chain.
    struct Node {
        Request data;       // The request.
        Node* next;         // Next (less urgent) request.
        explicit Node(const Request& request) : data(request), next(nullptr) {}
    };

    Node* head;   // The MOST urgent request.
    int count;    // Number of waiting requests.

public:
    PriorityRequestQueue() : head(nullptr), count(0) {}
    // Constructor: empty.

    ~PriorityRequestQueue();
    // Destructor: frees all nodes.

    PriorityRequestQueue(const PriorityRequestQueue&) = delete;
    PriorityRequestQueue& operator=(const PriorityRequestQueue&) = delete;
    // Copying is forbidden.

    bool isEmpty() const;                                       // true when nothing is waiting
    int  size() const;                                          // number of waiting requests
    void insert(const Request& request, bool verbose = true);   // insert in priority order     O(n)
    bool processHighestPriority(Request& processedRequest);     // remove the most urgent        O(1)
    bool peekHighest(Request& request) const;                   // look at the most urgent one
    void display() const;                                       // show all, most urgent first   O(n)
    Request* toArray(int& number) const;                        // copy to new[] array (caller must delete[])
};

#endif
