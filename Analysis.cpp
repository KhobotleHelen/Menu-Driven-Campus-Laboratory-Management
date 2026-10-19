// Analysis.cpp   (INTEGRATION)
// Prints the complexity analysis table and measures REAL running times, so the group can show that the
// theory (O(n) versus O(log n) versus O(1), O(n^2) versus O(n log n)) is visible in practice.
#include "Analysis.h"
#include "Laboratory.h"
#include "Searching.h"
#include "Sorting.h"
#include "BinarySearchTree.h"
#include "HashTable.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
#include <chrono>

using namespace std;

// One printed row of the complexity table.
static void row(const char* operation, const char* structure,
                const char* best, const char* average, const char* worst)
{
    cout << left << setw(30) << operation
         << setw(30) << structure
         << setw(10) << best
         << setw(12) << average
         << setw(10) << worst << '\n';
}

void showComplexityTable()
{
    cout << "\n=== COMPLEXITY ANALYSIS (n = items stored, V = locations, E = connections) ===\n\n";
    cout << left << setw(30) << "Operation" << setw(30) << "Data structure / algorithm"
         << setw(10) << "Best" << setw(12) << "Average" << setw(10) << "Worst" << '\n';
    cout << string(92, '-') << '\n';
    row("Enqueue request",            "Linked queue (rear pointer)", "O(1)",     "O(1)",       "O(1)");
    row("Dequeue request",            "Linked queue (front pointer)","O(1)",     "O(1)",       "O(1)");
    row("Push / Pop / Peek (undo)",   "Linked stack (top pointer)",  "O(1)",     "O(1)",       "O(1)");
    row("Insert priority request",    "Sorted linked list",          "O(1)",     "O(n)",       "O(n)");
    row("Process highest priority",   "Sorted linked list (head)",   "O(1)",     "O(1)",       "O(1)");
    row("Insert at front",            "Linked list",                 "O(1)",     "O(1)",       "O(1)");
    row("Insert at end",              "Linked list (no tail ptr)",   "O(1)",     "O(n)",       "O(n)");
    row("Insert at position / delete","Linked list",                 "O(1)",     "O(n)",       "O(n)");
    row("Search resource (unsorted)", "Linear search",               "O(1)",     "O(n)",       "O(n)");
    row("Search resource (sorted)",   "Binary search",               "O(1)",     "O(log n)",   "O(log n)");
    row("Sort resources",             "Bubble sort",                 "O(n)",     "O(n^2)",     "O(n^2)");
    row("Sort resources",             "Merge sort",                  "O(n log n)","O(n log n)","O(n log n)");
    row("Sort requests",              "Insertion sort",              "O(n)",     "O(n^2)",     "O(n^2)");
    row("Sort requests",              "Merge sort",                  "O(n log n)","O(n log n)","O(n log n)");
    row("Lookup by ID",               "Hash table (chaining)",       "O(1)",     "O(1)",       "O(n)");
    row("Insert / search / delete",   "Binary search tree",          "O(1)",     "O(log n)",   "O(n)");
    row("Tree traversal (in/pre/post)","Binary search tree",         "O(n)",     "O(n)",       "O(n)");
    row("BFS / DFS traversal",        "Graph (adjacency lists)",     "O(V+E)",   "O(V+E)",     "O(V+E)");
    row("Shortest path (fewest hops)","BFS on the graph",            "O(1)",     "O(V+E)",     "O(V+E)");
    row("Connected components",       "DFS on the graph",            "O(V+E)",   "O(V+E)",     "O(V+E)");
    cout << string(92, '-') << '\n';
    cout << "Notes:\n"
         << " - Bubble sort is O(n) in the best case only because it stops early when nothing was swapped.\n"
         << " - BST worst case O(n) happens when IDs are inserted already sorted (the tree becomes a chain).\n"
         << " - Hash table worst case O(n) happens only if every ID lands in the same bucket.\n"
         << " - Binary search needs sorted data, so sorting first costs O(n log n) once.\n"
         << " - Space: every structure stores n items, so O(n); BFS/DFS use O(V) extra space.\n";
}

// Elapsed microseconds between two clock readings.
static double microsBetween(chrono::steady_clock::time_point a, chrono::steady_clock::time_point b)
{
    return static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(b - a).count()) / 1000.0;
}

// Creates n laboratories with unique IDs in RANDOM order (random order keeps the BST reasonably balanced).
static Laboratory* makeBenchmarkLabs(int n)
{
    Laboratory* labs = new Laboratory[n];
    int* numbers = new int[n];
    for (int i = 0; i < n; i++) numbers[i] = i + 1;
    // numbers = 1, 2, ..., n.

    srand(12345);
    // Fixed seed: the same data every run (repeatable tests).

    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int t = numbers[i]; numbers[i] = numbers[j]; numbers[j] = t;
        // Fisher-Yates shuffle.
    }

    for (int i = 0; i < n; i++) {
        ostringstream id;
        id << "BEN" << setw(6) << setfill('0') << numbers[i];
        // IDs look like BEN000042.

        labs[i] = Laboratory(id.str(), "Lab " + id.str(), (i % 3 == 0 ? "Computer Laboratory" : "Laboratory"),
                             "Building " + string(1, static_cast<char>('A' + i % 5)),
                             1 + (i * 7) % 300, "Available", "Good");
    }
    delete[] numbers;
    return labs;
}

void runBenchmarks()
{
    const int sizes[] = {100, 1000, 5000};
    const int QUERIES = 200;
    // Every search test performs 200 lookups and reports the TOTAL time.

    cout << "\n=== PERFORMANCE MEASUREMENT (times in microseconds, generated data, fixed random seed) ===\n";
    cout << "This does NOT change your real data. Please wait a moment...\n\n";
    cout << left << setw(8) << "n"
         << setw(14) << "Linear x200" << setw(14) << "Binary x200"
         << setw(14) << "Hash x200" << setw(14) << "BST x200"
         << setw(16) << "Bubble sort" << setw(14) << "Merge sort" << '\n';
    cout << string(94, '-') << '\n';

    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        Laboratory* labs = makeBenchmarkLabs(n);
        // The unsorted data.

        string* keys = new string[QUERIES];
        for (int q = 0; q < QUERIES; q++) keys[q] = labs[(q * 37) % n].getId();
        // IDs to look for (all exist in the data).

        Laboratory* sorted = new Laboratory[n];
        for (int i = 0; i < n; i++) sorted[i] = labs[i];
        mergeSortById(sorted, 0, n - 1);
        // A sorted copy, required by binary search.

        HashTable* table = new HashTable();
        BinarySearchTree* tree = new BinarySearchTree();
        for (int i = 0; i < n; i++) { table->insert(labs[i]); tree->insert(labs[i]); }
        // Build the hash table and the BST (on the heap, to keep the stack small).

        int found = 0;
        // Counts successful searches (also stops the compiler from removing the loops).

        chrono::steady_clock::time_point t0 = chrono::steady_clock::now();
        for (int q = 0; q < QUERIES; q++) if (linearSearch(labs, n, keys[q]) != NULL) found++;
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        for (int q = 0; q < QUERIES; q++) if (binarySearch(sorted, n, keys[q]) != -1) found++;
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        for (int q = 0; q < QUERIES; q++) if (table->search(keys[q]) != NULL) found++;
        chrono::steady_clock::time_point t3 = chrono::steady_clock::now();
        for (int q = 0; q < QUERIES; q++) if (tree->search(keys[q]) != NULL) found++;
        chrono::steady_clock::time_point t4 = chrono::steady_clock::now();
        // Time each search method.

        Laboratory* copy1 = new Laboratory[n];
        for (int i = 0; i < n; i++) copy1[i] = labs[i];
        chrono::steady_clock::time_point t5 = chrono::steady_clock::now();
        bubbleSortBy(copy1, n, SORT_BY_ID);
        chrono::steady_clock::time_point t6 = chrono::steady_clock::now();
        // Bubble sort on a fresh copy.

        Laboratory* copy2 = new Laboratory[n];
        for (int i = 0; i < n; i++) copy2[i] = labs[i];
        chrono::steady_clock::time_point t7 = chrono::steady_clock::now();
        mergeSortBy(copy2, 0, n - 1, SORT_BY_ID);
        chrono::steady_clock::time_point t8 = chrono::steady_clock::now();
        // Merge sort on another fresh copy.

        cout << left << fixed << setprecision(1) << setw(8) << n
             << setw(14) << microsBetween(t0, t1) << setw(14) << microsBetween(t1, t2)
             << setw(14) << microsBetween(t2, t3) << setw(14) << microsBetween(t3, t4)
             << setw(16) << microsBetween(t5, t6) << setw(14) << microsBetween(t7, t8) << '\n';

        if (found != QUERIES * 4) cout << "  (warning: some searches failed)\n";

        delete[] labs; delete[] keys; delete[] sorted; delete[] copy1; delete[] copy2;
        delete table; delete tree;
        // Free everything we created.
    }

    cout << "\nHow to read this: when n grows 10x, linear search and the O(n^2) bubble sort grow much faster than\n"
         << "binary search, hash lookup, BST lookup and merge sort. This matches the complexity table.\n";
}
