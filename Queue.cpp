// Queue.cpp   (Student B)
// Implementation of the linked-list FIFO queue.
#include "Queue.h"

// Destructor: keep dequeuing until the queue is empty so every node is deleted.
RequestQueue::~RequestQueue() {
    Request ignored;
    // A throw-away request that receives the removed data.

    while (dequeue(ignored)) {}
    // dequeue() returns false when the queue is empty, which ends the loop.
}

// True when there is no front node.
bool RequestQueue::isEmpty() const {
    return frontNode == nullptr;
}

// Number of requests currently stored.
int RequestQueue::size() const {
    return count;
}

// ENQUEUE: add a request at the rear. O(1) because we keep a pointer to the rear.
void RequestQueue::enqueue(const Request& request, bool verbose) {
    Node* newNode = new Node(request);
    // Create a new node holding a copy of the request.

    if (isEmpty()) {
        // First request: it is both the front and the rear.

        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        // Link the old rear to the new node...

        rearNode = newNode;
        // ...and the new node becomes the rear.
    }

    count++;
    // One more request in the queue.

    if (verbose) {
        std::cout << "Request " << request.requestId << " added to the normal queue.\n";
        // Confirmation message (skipped while loading from a file).
    }
}

// DEQUEUE: remove the request at the front (the one that waited longest). O(1).
bool RequestQueue::dequeue(Request& removedRequest) {
    if (isEmpty()) {
        return false;
        // Nothing to remove.
    }

    Node* oldFront = frontNode;
    // Remember the node we are about to delete.

    removedRequest = oldFront->data;
    // Hand the request data back to the caller.

    frontNode = frontNode->next;
    // The second node becomes the new front.

    if (frontNode == nullptr) {
        rearNode = nullptr;
        // The queue became empty, so the rear pointer must be cleared too.
    }

    delete oldFront;
    // Free the old node.

    count--;
    // One request fewer.

    return true;
    // Success.
}

// PEEK: read the front request without removing it.
bool RequestQueue::peekFront(Request& frontRequest) const {
    if (isEmpty()) {
        return false;
    }
    frontRequest = frontNode->data;
    return true;
}

// DISPLAY: walk from the front to the rear and print each request.
void RequestQueue::display(const char* title) const {
    if (isEmpty()) {
        std::cout << "There are no requests in this list.\n";
        // Friendly message for the empty-queue boundary case.
        return;
    }

    std::cout << "\n--- " << title << " ---\n";
    Node* current = frontNode;
    // Start at the front.

    int position = 1;
    // Position in the queue (1 = next to be served).

    while (current != nullptr) {
        std::cout << "#" << position
                  << " | ID: " << current->data.requestId
                  << " | User: " << current->data.studentId
                  << " | Resource: " << current->data.laboratory
                  << " | Item: " << current->data.item
                  << " | Type: " << current->data.requestType
                  << " | Priority: " << current->data.priority
                  << " | Time: " << current->data.requestTime
                  << " | Status: " << current->data.status << '\n';
        // Print every field of the request on one line.

        current = current->next;
        // Move towards the rear.

        position++;
    }
}

// TOARRAY: copy all requests into a new dynamic array (front first). Used by sorting and saving.
Request* RequestQueue::toArray(int& number) const {
    number = count;
    // Tell the caller how many requests there are.

    if (number == 0) {
        return nullptr;
        // Nothing to copy.
    }

    Request* array = new Request[number];
    // Allocate the array; the caller must delete[] it.

    Node* current = frontNode;
    int index = 0;
    while (current != nullptr) {
        array[index++] = current->data;
        // Copy one request.

        current = current->next;
        // Next node.
    }
    return array;
}
