// Sorting.h   (Student A)
// Declares Bubble Sort  O(n^2)  and  Merge Sort  O(n log n).
// INTEGRATION CHANGE: the brief requires sorting by ID, Name, Capacity AND Type, so both
// algorithms now accept a "sortKey". The original "ById" functions are kept as wrappers.

#ifndef SORTING_H
#define SORTING_H

#include "Laboratory.h"
// Sorting algorithms work with Laboratory objects.

// The four sorting criteria required by the brief (Module 6).
const int SORT_BY_ID       = 1;
const int SORT_BY_NAME     = 2;
const int SORT_BY_CAPACITY = 3;
const int SORT_BY_TYPE     = 4;

int compareLabs(const Laboratory& a, const Laboratory& b, int sortKey);
// Returns a negative number if a comes first, 0 if equal, a positive number if b comes first.
// When two laboratories tie on the chosen key, the ID is used as a tie-breaker.

void bubbleSortBy(Laboratory array[], int size, int sortKey);
// Bubble Sort arranges laboratories by the chosen key.   Time: O(n^2)

void mergeSortBy(Laboratory array[], int left, int right, int sortKey);
// Merge Sort arranges laboratories by the chosen key.    Time: O(n log n)

void bubbleSortById(Laboratory array[], int size);
// Original Student A function: Bubble Sort by ID.

void mergeSortById(Laboratory array[], int left, int right);
// Original Student A function: Merge Sort by ID.

void mergeHalves(Laboratory array[], int left, int middle, int right, int sortKey);
// Combines two sorted sections (originally called merge(); renamed to avoid a clash with std::merge).

#endif
