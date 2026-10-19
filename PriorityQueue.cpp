// PriorityQueue.cpp   (Student B)
// Implementation of the priority queue (sorted linked list).
#include "PriorityQueue.h"

// Destructor: remove everything so no memory is leaked.
PriorityRequestQueue::~PriorityRequestQueue() {
    Request ignored;
    while (processHighestPriority(ignored)) {}
}

// Empty when the head is NULL.
bool PriorityRequestQueue::isEmpty() const {
    return head == nullptr;
}

// Number of waiting requests.
int PriorityRequestQueue::size() const {
    return count;
}

// INSERT: put the new request in its correct place so the list stays sorted by priority.
void PriorityRequestQueue::insert(const Request& request, bool verbose) {
    Node* newNode = new Node(request);
    // Create a node for the new request.

    // Smaller priority number means greater priority.
    // Insert after existing requests with the same priority to preserve their order.
    if (head == nullptr || request.priority < head->data.priority) {
        // Case 1: list is empty OR the new request is more urgent than everything: it becomes the head.

        newNode->next = head;
        head = newNode;
    } else {
        // Case 2: walk down the list until we reach the last request that is at least as urgent as the new one.

        Node* current = head;
        while (current->next != nullptr &&
               current->next->data.priority <= request.priority) {
            current = current->next;
            // Skip every request that is as urgent or more urgent than the new one.
        }
        newNode->next = current->next;
        // The new node points at the first less-urgent request...

        current->next = newNode;
        // ...and the previous node points at the new node.
    }

    count++;
    // One more request.

    if (verbose) {
        std::cout << "Request " << request.requestId
                  << " added to the priority queue.\n";
    }
}

// PROCESS: remove the request at the head (the most urgent). O(1).
bool PriorityRequestQueue::processHighestPriority(Request& processedRequest) {
    if (isEmpty()) {
        return false;
        // Nothing waiting.
    }

    Node* oldHead = head;
    // Remember the node to delete.

    processedRequest = oldHead->data;
    // Give the request to the caller.

    head = head->next;
    // The next most urgent request becomes the head.

    delete oldHead;
    // Free the old head.

    count--;
    return true;
}

// PEEK: look at the most urgent request without removing it.
bool PriorityRequestQueue::peekHighest(Request& request) const {
    if (isEmpty()) {
        return false;
    }
    request = head->data;
    return true;
}

// DISPLAY: most urgent first.
void PriorityRequestQueue::display() const {
    if (isEmpty()) {
        std::cout << "There are no priority requests waiting.\n";
        return;
    }

    std::cout << "\n--- Priority Requests (highest priority first) ---\n";
    Node* current = head;
    while (current != nullptr) {
        std::cout << "ID: " << current->data.requestId
                  << " | User: " << current->data.studentId
                  << " | Resource: " << current->data.laboratory
                  << " | Item: " << current->data.item
                  << " | Type: " << current->data.requestType
                  << " | Priority: " << current->data.priority
                  << " | Time: " << current->data.requestTime
                  << " | Status: " << current->data.status << '\n';
        current = current->next;
    }
}

// TOARRAY: copy all requests (most urgent first) into a new array. Caller must delete[] it.
Request* PriorityRequestQueue::toArray(int& number) const {
    number = count;
    if (number == 0) {
        return nullptr;
    }
    Request* array = new Request[number];
    Node* current = head;
    int index = 0;
    while (current != nullptr) {
        array[index++] = current->data;
        current = current->next;
    }
    return array;
}
