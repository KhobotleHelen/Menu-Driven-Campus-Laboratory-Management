// Sorting.cpp   (Student A)
// This file implements Bubble Sort and Merge Sort.
#include "Sorting.h"
// means Include our sorting declarations.

// Compares two laboratories using the chosen key.
int compareLabs(const Laboratory& a, const Laboratory& b, int sortKey)
{
    if (sortKey == SORT_BY_NAME)
    {
        // Sort alphabetically by name.

        if (a.getName() < b.getName()) return -1;
        // a's name comes first.

        if (a.getName() > b.getName()) return 1;
        // b's name comes first.
    }
    else if (sortKey == SORT_BY_CAPACITY)
    {
        // Sort numerically by capacity (smallest first).

        if (a.getCapacity() < b.getCapacity()) return -1;
        // a has the smaller capacity.

        if (a.getCapacity() > b.getCapacity()) return 1;
        // b has the smaller capacity.
    }
    else if (sortKey == SORT_BY_TYPE)
    {
        // Sort alphabetically by laboratory type.

        if (a.getType() < b.getType()) return -1;
        // a's type comes first.

        if (a.getType() > b.getType()) return 1;
        // b's type comes first.
    }

    // Either the key is SORT_BY_ID or the two laboratories tied: compare IDs.
    if (a.getId() < b.getId()) return -1;
    // a's ID comes first.

    if (a.getId() > b.getId()) return 1;
    // b's ID comes first.

    return 0;
    // They are completely equal.
}

// Bubble Sort with a sort key.
void bubbleSortBy(Laboratory array[], int size, int sortKey)
{
    for (int i = 0; i < size - 1; i++)
    {
        // Repeat passes through the array.

        bool swapped = false;
        // Remembers whether this pass changed anything.

        for (int j = 0; j < size - i - 1; j++)
        {
            // Compare neighbouring laboratories.

            if (compareLabs(array[j], array[j + 1], sortKey) > 0)
            {
                // If they are in the wrong order

                Laboratory temporary = array[j];
                // Temporarily store the first Laboratory.

                array[j] = array[j + 1];
                // Move the second Laboratory into the first position.

                array[j + 1] = temporary;
                // Move the first Laboratory into the second position.

                swapped = true;
                // Something moved during this pass.
            }
        }

        if (!swapped) break;
        // If nothing moved, the array is already sorted: stop early (best case O(n)).
    }
}

// Original Student A wrapper.
void bubbleSortById(Laboratory array[], int size)
{
    bubbleSortBy(array, size, SORT_BY_ID);
    // Bubble Sort using the ID as the key.
}

// Merge Sort with a sort key.
void mergeSortBy(Laboratory array[], int left, int right, int sortKey)
{
    if (left < right)
    {
        // Continue only when there is more than one item.

        int middle = left + (right - left) / 2;
        // Find the middle of the current section.

        mergeSortBy(array, left, middle, sortKey);
        // Sort the left half.

        mergeSortBy(array, middle + 1, right, sortKey);
        // Sort the right half.

        mergeHalves(array, left, middle, right, sortKey);
        // Combine the two sorted halves.
    }
}

// Original Student A wrapper.
void mergeSortById(Laboratory array[], int left, int right)
{
    mergeSortBy(array, left, right, SORT_BY_ID);
    // Merge Sort using the ID as the key.
}

//Merge two sorted halves
void mergeHalves(Laboratory array[], int left, int middle, int right, int sortKey)
{
    int leftSize = middle - left + 1;
    // Calculate the size of the left half.

    int rightSize = right - middle;
    // Calculate the size of the right half.

    Laboratory* leftArray = new Laboratory[leftSize];
    // Dynamically create an array for the left half.

    Laboratory* rightArray = new Laboratory[rightSize];
    // Dynamically create an array for the right half.

    for (int i = 0; i < leftSize; i++)
    {
        // Copy the left half.

        leftArray[i] = array[left + i];
        // Store each left-side Laboratory.
    }

    for (int j = 0; j < rightSize; j++)
    {
        // Copy the right half.

        rightArray[j] = array[middle + 1 + j];
        // Store each right-side Laboratory.
    }

    int i = 0;
    // Start of left array.

    int j = 0;
    // Start of right array.

    int k = left;
    // Start position in the original array.

    while (i < leftSize && j < rightSize)
    {
        // Continue while both halves still contain items.

        if (compareLabs(leftArray[i], rightArray[j], sortKey) <= 0)
        {
            // If the left laboratory should come first (<= keeps the sort stable)...

            array[k] = leftArray[i];
            // Put the left Laboratory into the original array.

            i++;
            // Move to the next left Laboratory.
        }
        else
        {
            // Otherwise the right Laboratory comes first.

            array[k] = rightArray[j];
            // Put the right Laboratory into the original array.

            j++;
            // Move to the next right Laboratory.
        }

        k++;
        // Move to the next position in the original array.
    }

    while (i < leftSize)
    {
        // Copy any remaining left-side laboratories.

        array[k] = leftArray[i];
        // Copy the remaining Laboratory.

        i++;
        // Move forward.

        k++;
        // Move forward in the original array.
    }

    while (j < rightSize)
    {
        // Copy any remaining right-side laboratories.

        array[k] = rightArray[j];
        // Copy the remaining Laboratory.

        j++;
        // Move forward.

        k++;
        // Move forward in the original array.
    }

    delete[] leftArray;
    // Free the temporary left array.

    delete[] rightArray;
    // Free the temporary right array.
}
