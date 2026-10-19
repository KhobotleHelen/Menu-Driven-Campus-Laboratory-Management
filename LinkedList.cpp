// This file contains the implementation of our custom Linked List.
#include "LinkedList.h"
#include <iostream>
#include <string>
using namespace std;

LinkedList::LinkedList()
{
    head = NULL;
    // The list starts empty, so there is no first Node.

    count = 0;
    // There are zero laboratories initially.
}

//copy constructor
LinkedList::LinkedList(const LinkedList& other)
{
    head = NULL;
    // Start the new list as empty.

    count = 0;
    // The new list initially contains zero Nodes.

    Node* current = other.head;
    // Start reading the other list from its first Node.

    while (current != NULL)
    {
        addEnd(current->data);
        // Copy each Laboratory into a new Node in our list.

        current = current->next;
        // Move to the next Node in the original list.
    }
}
//Destructor
LinkedList::~LinkedList()
{
    clear();
    // Delete all Nodes before the Linked List disappears.
}
//Assignment Operator
LinkedList& LinkedList::operator=(const LinkedList& other)
{
    if (this != &other)
    {
        // Checks that we are not assigning a list to itself.

        clear();
        // Delete our existing Nodes first.

        Node* current = other.head;
        // Start at the first Node of the other list.

        while (current != NULL)
        {
            addEnd(current->data);
            // Create a new Node containing the Laboratory.

            current = current->next;
            // Move to the next Node.
        }
    }

    return *this;
    // Return the current Linked List.
}

//isEmpty
bool LinkedList::isEmpty() const
{
    return head == NULL;
    // If head is NULL, there is no first Node.
    // Therefore the list is empty.
}
int LinkedList::size() const
{
    return count;
    // Return the number of laboratories.
}
void LinkedList::addFront(Laboratory lab)
{
    Node* newNode = new Node(lab);
    // Dynamically create a new Node containing the Laboratory.

    newNode->next = head;
    // Make the new Node point to the old first Node.

    head = newNode;
    // Make the new Node the first Node.

    count++;
    // One laboratory has been added.
}
void LinkedList::addEnd(Laboratory lab)
{
    Node* newNode = new Node(lab);
    // Create a new Node containing the Laboratory.

    if (head == NULL)
    {
        // Check whether the list is empty.

        head = newNode;
        // If empty, the new Node becomes the first Node.

        count++;
        // Increase the number of laboratories.

        return;
        // We are finished.
    }

    Node* current = head;
    // Start at the first Node.

    while (current->next != NULL)
    {
        // Continue until current reaches the last Node.

        current = current->next;
        // Move to the next Node.
    }

    current->next = newNode;
    // Connect the last Node to the new Node.

    count++;
    // Increase the number of laboratories.
}
bool LinkedList::addAtPosition(Laboratory lab, int position)
{
    if (position <= 1)
    {
        // Position 1 means the beginning.

        addFront(lab);
        // Reuse the addFront function.

        return true;
        // Stop because the laboratory has already been added (success).
    }

    Node* newNode = new Node(lab);
    // Create the new Node.

    Node* current = head;
    // Start from the first Node.

    for (int i = 1; i < position - 1 && current != NULL; i++)
    {
        // Move current until it reaches the Node
        // immediately before the requested position.

        current = current->next;
        // Move to the next Node.
    }

    if (current == NULL)
    {
        // The requested position does not exist.

        delete newNode;
        // Delete the Node we created because we will not use it.

        return false;
        // Stop the function and report failure.
    }

    newNode->next = current->next;
    // First make the new Node point to the Node after current.

    current->next = newNode;
    // Now make current point to the new Node.

    count++;
    // One laboratory has been added.

    return true;
    // Tell the caller the insertion succeeded.
}
void LinkedList::display() const
{
    if (head == NULL)
    {
        // Check whether there are any laboratories.

        cout << "No laboratories found." << endl;
        // Tell the user the list is empty.

        return;
        // Stop the function.
    }

    Node* current = head;
    // Start at the first Node.

    int position = 1;
    // Start counting positions from 1.

    while (current != NULL)
    {
        // Continue until there are no more Nodes.

        cout << "\nPosition: " << position << endl;
        // Display the current position.

        cout << "ID: " << current->data.getId() << endl;
        // Display the laboratory ID.

        cout << "Name: " << current->data.getName() << endl;
        // Display the laboratory name.

        cout << "Type: " << current->data.getType() << endl;
        // Display the laboratory type.

        cout << "Building: " << current->data.getBuilding() << endl;
        // Display the laboratory building.

        cout << "Capacity: " << current->data.getCapacity() << endl;
        // Display the laboratory capacity.

        cout << "Status: " << current->data.getStatus() << endl;
        // Display the laboratory status.

        cout << "Condition: " << current->data.getCondition() << endl;
        // Display the laboratory condition.

        cout << "-----------------------------" << endl;
        // Separate this laboratory from the next one.

        current = current->next;
        // Move to the next Node.

        position++;
        // Increase the position number.
    }
}
bool LinkedList::removeById(string id)
{
    if (head == NULL)
    {
        // There is nothing to remove.

        return false;
        // Tell the caller that removal failed.
    }

    if (head->data.getId() == id)
    {
        // Check whether the first Node contains the requested ID.

        Node* temporary = head;
        // Remember the Node we are about to delete.

        head = head->next;
        // Move head to the second Node.

        delete temporary;
        // Delete the old first Node from memory.

        count--;
        // One laboratory was removed.

        return true;
        // Tell the caller removal succeeded.
    }

    Node* current = head;
    // Start at the first Node.

    while (current->next != NULL)
    {
        // Continue while another Node exists.

        if (current->next->data.getId() == id)
        {
            // Check the Node after current.

            Node* temporary = current->next;
            // Save the Node that needs to be deleted.

            current->next = temporary->next;
            // Skip over the Node being deleted.

            delete temporary;
            // Free its memory.

            count--;
            // One laboratory was removed.

            return true;
            // Tell the caller removal succeeded.
        }

        current = current->next;
        // Move to the next Node.
    }

    return false;
    // The ID was not found.
}
bool LinkedList::updateById(string id, Laboratory updatedLab)
{
    Node* current = head;
    // Start at the first Node.

    while (current != NULL)
    {
        // Continue until the end.

        if (current->data.getId() == id)
        {
            // Check whether this is the laboratory we want.

            current->data = updatedLab;
            // Replace the old Laboratory with the updated one.

            return true;
            // Tell the caller the update succeeded.
        }

        current = current->next;
        // Move to the next Node.
    }

    return false;
    // The laboratory was not found.
}
Laboratory* LinkedList::searchById(string id)
{
    Node* current = head;
    // Start at the first Node.

    while (current != NULL)
    {
        // Continue through the list.

        if (current->data.getId() == id)
        {
            // Compare the current laboratory ID with the requested ID.

            return &current->data;
            // Return the address of the matching Laboratory.
        }

        current = current->next;
        // Move to the next Node.
    }

    return NULL;
    // The laboratory was not found.
}
// INTEGRATION ADDITION (needed by UNDO)
// Returns the 1-based position of a laboratory ID, or 0 when it is not found.
int LinkedList::positionOfId(string id) const
{
    Node* current = head;
    // Start at the first Node.

    int position = 1;
    // Positions are counted from 1, like the menu.

    while (current != NULL)
    {
        // Walk through the list.

        if (current->data.getId() == id)
        {
            // Found the requested ID.

            return position;
            // Give the position back.
        }

        current = current->next;
        // Move to the next Node.

        position++;
        // Increase the position counter.
    }

    return 0;
    // 0 means "not in the list".
}
void LinkedList::clear()
{
    while (head != NULL)
    {
        // Continue while at least one Node exists.

        Node* temporary = head;
        // Remember the current first Node.

        head = head->next;
        // Move head to the next Node.

        delete temporary;
        // Delete the old Node.
    }

    count = 0;
    // The list is now empty.
}
Laboratory* LinkedList::toArray(int& number) const
{
    number = count;
    // Tell the caller how many laboratories exist.

    if (number == 0)
    {
        // If there are no laboratories...

        return NULL;
        // there is no array to create.
    }

    Laboratory* array = new Laboratory[number];
    // Dynamically create an array large enough for all laboratories.

    Node* current = head;
    // Start at the first Node.

    int index = 0;
    // Arrays begin at index 0.

    while (current != NULL)
    {
        // Continue through every Node.

        array[index] = current->data;
        // Copy the Laboratory into the array.

        index++;
        // Move to the next array position.

        current = current->next;
        // Move to the next Node.
    }

    return array;
    // Give the caller the dynamically created array.
}

