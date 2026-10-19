// Searching.h   (Student A)
// Declares the searching algorithms: Linear Search and Binary Search.
// INTEGRATION CHANGE: every function has an OPTIONAL "steps" pointer. When it is not NULL,
// the function stores how many comparisons it made. The menu prints this so the group can
// DEMONSTRATE O(n) versus O(log n) during the presentation.

#ifndef SEARCHING_H
#define SEARCHING_H
#include "Laboratory.h"
// Searching works with Laboratory objects.

Laboratory* linearSearch(Laboratory array[], int size, string id, int* steps = NULL);
// Linear search checks laboratories one by one by ID.   Time: O(n)

Laboratory* linearSearchByName(Laboratory array[], int size, string name, int* steps = NULL);
// INTEGRATION ADDITION: linear search by laboratory NAME (names are not sorted, so linear is the right tool).

int binarySearch(Laboratory array[], int size, string id, int* steps = NULL);
// Binary search repeatedly divides a SORTED (by ID) array in half.   Time: O(log n)
// It returns the index if found and -1 if not found.

#endif
