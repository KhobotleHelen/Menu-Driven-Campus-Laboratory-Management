// Searching.cpp   (Student A)
// Implements linear and binary searching.
#include "Searching.h"
// Include the declarations of our searching functions.

//Linear Search by ID
Laboratory* linearSearch(Laboratory array[], int size, string id, int* steps)
{
    int comparisons = 0;
    // Counts how many laboratories we looked at.

    for (int i = 0; i < size; i++)
    {
        // Check every laboratory from the beginning to the end.

        comparisons++;
        // One more comparison is about to happen.

        if (array[i].getId() == id)
        {
            // Compare the current laboratory's ID with the requested ID.

            if (steps != NULL) *steps = comparisons;
            // Report the number of comparisons if the caller asked for it.

            return &array[i];
            // Return the address of the matching Laboratory.
        }
    }

    if (steps != NULL) *steps = comparisons;
    // Report the comparisons even when nothing was found.

    return NULL;
    // The ID was not found.
}

//Linear Search by Name
Laboratory* linearSearchByName(Laboratory array[], int size, string name, int* steps)
{
    int comparisons = 0;
    // Counts how many laboratories we looked at.

    for (int i = 0; i < size; i++)
    {
        // Check every laboratory from the beginning to the end.

        comparisons++;
        // One more comparison.

        if (array[i].getName() == name)
        {
            // Compare the names.

            if (steps != NULL) *steps = comparisons;
            // Report the comparison count.

            return &array[i];
            // Return the address of the matching Laboratory.
        }
    }

    if (steps != NULL) *steps = comparisons;
    // Report the comparison count.

    return NULL;
    // The name was not found.
}

//Binary Search (the array MUST already be sorted by ID)
int binarySearch(Laboratory array[], int size, string id, int* steps)
{
    int comparisons = 0;
    // Counts how many middle elements we examined.

    int low = 0;
    // low represents the first possible position.

    int high = size - 1;
    // high represents the last possible position.

    while (low <= high)
    {
        // Continue while there is still a possible search area.

        int middle = low + (high - low) / 2;
        // Find the middle position (written this way to avoid integer overflow).

        comparisons++;
        // We examine one middle element.

        if (array[middle].getId() == id)
        {
            // Check whether the middle laboratory is the one we want.

            if (steps != NULL) *steps = comparisons;
            // Report the comparison count.

            return middle;
            // Return its position.
        }

        if (array[middle].getId() < id)
        {
            // If the middle ID comes before the requested ID...

            low = middle + 1;
            // ...ignore the left half.
        }
        else
        {
            // Otherwise the requested ID is before the middle.

            high = middle - 1;
            // Ignore the right half.
        }
    }

    if (steps != NULL) *steps = comparisons;
    // Report the comparison count.

    return -1;
    // The ID was not found.
}
