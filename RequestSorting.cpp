// RequestSorting.cpp   (Student B)
#include "RequestSorting.h"

// Compare two requests using the chosen key.
int compareRequests(const Request& a, const Request& b, int sortKey)
{
    if (sortKey == REQ_BY_TIME) {
        if (a.requestTime < b.requestTime) return -1;
        if (a.requestTime > b.requestTime) return 1;
        // The timestamp format (YYYY-MM-DD HH:MM:SS) sorts correctly as plain text.
    } else if (sortKey == REQ_BY_PRIORITY) {
        if (a.priority < b.priority) return -1;
        if (a.priority > b.priority) return 1;
    } else if (sortKey == REQ_BY_STATUS) {
        if (a.status < b.status) return -1;
        if (a.status > b.status) return 1;
    }

    // Tie-breaker: the request ID (lower ID = submitted earlier).
    if (a.requestId < b.requestId) return -1;
    if (a.requestId > b.requestId) return 1;
    return 0;
}

// INSERTION SORT: take each element and slide it left until it is in the right place.
void insertionSortRequests(Request array[], int size, int sortKey)
{
    for (int i = 1; i < size; i++) {
        Request key = array[i];
        // The element we are inserting into the already-sorted left part.

        int j = i - 1;
        while (j >= 0 && compareRequests(array[j], key, sortKey) > 0) {
            array[j + 1] = array[j];
            // Shift larger elements one place to the right.

            j--;
        }
        array[j + 1] = key;
        // Drop the element into the gap.
    }
}

// Merge two sorted halves (helper for merge sort).
static void mergeRequestHalves(Request array[], int left, int middle, int right, int sortKey)
{
    int leftSize = middle - left + 1;
    int rightSize = right - middle;
    Request* leftArray = new Request[leftSize];
    Request* rightArray = new Request[rightSize];
    // Temporary copies of the two halves.

    for (int i = 0; i < leftSize; i++) leftArray[i] = array[left + i];
    for (int j = 0; j < rightSize; j++) rightArray[j] = array[middle + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < leftSize && j < rightSize) {
        if (compareRequests(leftArray[i], rightArray[j], sortKey) <= 0) {
            array[k++] = leftArray[i++];
            // Left element is first (<= keeps the sort stable).
        } else {
            array[k++] = rightArray[j++];
        }
    }
    while (i < leftSize) array[k++] = leftArray[i++];
    while (j < rightSize) array[k++] = rightArray[j++];
    // Copy whatever is left in either half.

    delete[] leftArray;
    delete[] rightArray;
    // Free the temporary arrays.
}

// MERGE SORT: split in half, sort each half, merge.
void mergeSortRequests(Request array[], int left, int right, int sortKey)
{
    if (left < right) {
        int middle = left + (right - left) / 2;
        mergeSortRequests(array, left, middle, sortKey);
        mergeSortRequests(array, middle + 1, right, sortKey);
        mergeRequestHalves(array, left, middle, right, sortKey);
    }
}
