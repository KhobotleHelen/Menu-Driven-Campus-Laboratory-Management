# NUL Smart Laboratory Management System
### CS3400 Data Structures and Algorithms — Group Mini Project (C++)

**Scenario:** Campus Facilities (laboratories, requests, activities, campus map)
**Language:** C++11 (no external libraries; the data structures are written by the group, not taken from the STL)

---

## 1. Group Members

| Role | Name | Student Number |
|------|------|----------------|
| Student A – Core Data Management |Khobotle Kabelo|
| Student B – Request and Activity Management |Mathe Pule|
| Student C – Advanced Data Structures and Navigation |Matsoso P|

**Group number:** 3   **Lecturer:** Balone Ndaba

---

## 2. What is in this folder

```
CS3400_Campus_Lab_System/
├── main.cpp                  program entry point + main menu
├── Menus.h / Menus.cpp       all ten menus (integration)
├── CampusSystem.h            one object holding every data structure
├── ResourceManager.h/.cpp    keeps linked list + BST + hash table in step (integration)
├── InputHelper.h/.cpp        safe keyboard input (integration)
├── Analysis.h/.cpp           complexity table + performance measurements (integration)
│
├── Laboratory.h/.cpp         STUDENT A  resource class (shared by everyone)
├── Node.h/.cpp               STUDENT A  linked list node
├── LinkedList.h/.cpp         STUDENT A  custom singly linked list
├── Searching.h/.cpp          STUDENT A  linear + binary search
├── Sorting.h/.cpp            STUDENT A  bubble sort + merge sort
├── FileManager.h/.cpp        STUDENT A  load / save all data files
│
├── Request.h                 STUDENT B  request record
├── Queue.h/.cpp              STUDENT B  FIFO request queue (linked)
├── Stack.h/.cpp              STUDENT B  undo stack (linked)
├── PriorityQueue.h/.cpp      STUDENT B  priority request queue (linked, sorted)
├── RequestSorting.h/.cpp     STUDENT B  insertion sort + merge sort for requests
├── Activity.h/.cpp           STUDENT B  activity scheduler with clash detection
│
├── BinarySearchTree.h/.cpp   STUDENT C  BST keyed by laboratory ID
├── HashTable.h/.cpp          STUDENT C  hash table, separate chaining
├── CampusGraph.h/.cpp        STUDENT C  campus map, BFS, DFS, shortest path, components
│
├── data/                     sample data (loaded at start, saved at exit)
│   ├── resources.txt   requests.txt   locations.txt   activities.txt
│   └── generate_large_data.py   (makes a big file for the large-data test)
├── tests/                    unit tests + end-to-end test
├── docs/                     technical report (PDF), integration notes, demo script, test results
├── Makefile                  build with make (Linux / macOS / MinGW)
├── build.bat  run.bat        build and run on Windows with one double-click
└── README.md                 this file
```

---
## 4. Using the program (main menu = Section 28 of the brief)

| Menu | What it does | Data structure / algorithm |
|------|--------------|----------------------------|
| 1 Resource Management | add at beginning / end / position, display, remove, update, save, reload | **Linked list** (Student A) |
| 2 Search Resources | linear search by ID or name, binary search by ID, compare both (shows the number of comparisons) | Linear **O(n)**, Binary **O(log n)** |
| 3 Sort Resources | sort labs by ID / name / capacity / type; sort requests by time / priority / status | Bubble **O(n²)**, Merge **O(n log n)**, Insertion |
| 4 Resource Requests | submit, view, process (FIFO), peek, history; schedule and view activities | **Queue**; ordered linked list |
| 5 Priority Requests | submit with priority 1–4, view, process most urgent first | **Priority queue** |
| 6 Undo Operations | view top, undo, record note, stack status | **Stack** (really restores deleted / edited labs) |
| 7 Tree Operations | inorder / preorder / postorder, search, statistics | **Binary search tree** |
| 8 Hash Table Lookup | find lab by ID, show buckets, statistics | **Hash table**, separate chaining |
| 9 Campus Navigation | display map, BFS, DFS, connectivity, shortest path, components, add location / connection | **Graph** (adjacency lists) |
| 10 System Analysis | complexity table, live performance measurements, system statistics | Algorithm analysis |
| 0 Exit | saves everything and quits | File persistence |

**How the parts work together:** every laboratory is stored in a linked list (master copy), a BST (ordered index) and a hash table (instant lookup). `ResourceManager` keeps the three identical. A request is checked against the hash table when submitted; when it is processed, an *Available* lab becomes *Booked* (or *Maintenance* for a maintenance request) in all three structures, and the change can be undone from the stack.


---

## 5. Data files (`data/`)

| File | One line looks like | Notes |
|------|---------------------|-------|
| `resources.txt` | `LAB101\|ICT Lab\|Computer Laboratory\|Building 32\|40\|Available\|Good` | id, name, type, building, capacity, status, condition |
| `requests.txt` | `5\|Dr Nkosi\|LAB310\|Fume cupboard failure\|maintenance\|1\|Pending\|2026-10-12 09:10:00\|P` | last field: `N` normal queue, `P` priority queue, `H` processed history |
| `locations.txt` | `LOCATION\|Library` and `EDGE\|Library\|Academic Building` | graph vertices and pathways |
| `activities.txt` | `1\|C++ practical test\|LAB101\|2026-11-02\|09:00\|2` | id, name, venue, date, time, priority |

The `|` character separates fields, so it cannot be typed inside a field (the program replaces it with `/`). Files are loaded when the program starts and saved when you choose **0. Exit** (or Menu 1 → 8). If you close the window without choosing Exit, changes are **not** saved.
---
---

## 7. External code and academic integrity

All data structures and algorithms were written by the group members (Students A, B and C). No external libraries were used other than the C++ standard library for input/output, strings and timing. The merge of the three parts into one program is described in `docs/INTEGRATION_NOTES.md`.

We confirm that every member can explain the whole system.
