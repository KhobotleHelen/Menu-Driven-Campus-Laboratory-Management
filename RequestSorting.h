// RequestSorting.h   (Student B)
// Sorting algorithms for REQUESTS (Module 6 says requests may be sorted by
// submission time, priority and status).
//   Insertion Sort  O(n^2) average, O(n) when nearly sorted, stable, in place.
//   Merge Sort      O(n log n) always, stable, needs O(n) extra space.
#ifndef REQUEST_SORTING_H
#define REQUEST_SORTING_H

#include "Request.h"

const int REQ_BY_TIME     = 1;   // submission time, oldest first
const int REQ_BY_PRIORITY = 2;   // priority number, 1 (emergency) first
const int REQ_BY_STATUS   = 3;   // status text, alphabetical

int compareRequests(const Request& a, const Request& b, int sortKey);
// Negative: a first. 0: equal. Positive: b first. Ties are broken by request ID so the order is predictable.

void insertionSortRequests(Request array[], int size, int sortKey);
void mergeSortRequests(Request array[], int left, int right, int sortKey);

#endif
