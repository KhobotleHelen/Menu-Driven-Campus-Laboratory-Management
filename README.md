# NUL Smart Laboratory Management System
### CS3400 Data Structures and Algorithms — Group Mini Project (C++)

**Scenario:** Campus Facilities (laboratories, requests, activities, campus map)
**Language:** C++11 (no external libraries; the data structures are written by the group, not taken from the STL)

---

## 1. Group Members

| Role | Name | Student Number |
|------|------|----------------|
| Student A – Core Data Management | [ full name ] | [ number ] |
| Student B – Request and Activity Management | [ full name ] | [ number ] |
| Student C – Advanced Data Structures and Navigation | [ full name ] | [ number ] |

**Group number:** [ ]   **Lecturer:** [ ]

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

## 3. How to compile and run on YOUR laptop

> **Important:** the program must be started **from the project folder**, because it reads and writes the `data/` folder using a relative path.
> The compiler must use the **C++11** standard (`-std=c++11`). Older settings (the default of old Dev-C++) will fail with errors about `nullptr`, `= delete` or `to_string`.

### Option A — Windows with g++ / MinGW (easiest)
1. Extract the zip. Open the folder.
2. Double-click **`build.bat`**. Wait for `BUILD SUCCESSFUL`.
3. Double-click **`run.bat`**. The menu appears.

(If `build.bat` says g++ was not found, install MinGW-w64 or use Option B.)

### Option B — Dev-C++ (the IDE your group already uses)
1. Open Dev-C++ → **File → New → Project → Console Application → C++ Project**. Name it `CampusSystem` and save it **inside the extracted project folder** (the same folder as `main.cpp` and `data`).
2. Delete the `main.cpp` that Dev-C++ created, then **Project → Add to Project** and select **all `.cpp` files** from the project folder (not the ones in `tests/`).
3. **Tools → Compiler Options** → tick *“Add the following commands when calling the compiler”* and type: `-std=c++11` → OK.
4. **Execute → Rebuild All** (F12). The `.exe` must be created in the project folder.
5. Start the program by double-clicking the `.exe` **inside the project folder** (or use `run.bat` after renaming/copying the exe to `campus_system.exe`). If the window closes immediately, open *Command Prompt*, `cd` to the project folder and run `campus_system.exe`.

### Option C — Linux / macOS / WSL / MinGW with make
```bash
make          # builds ./campus_system
./campus_system
```
or just `./run.sh`.

### Option D — one command (any g++)
```bash
g++ -std=c++11 -Wall -Wextra -pedantic -o campus_system main.cpp Menus.cpp Analysis.cpp InputHelper.cpp ResourceManager.cpp Laboratory.cpp Node.cpp LinkedList.cpp Searching.cpp Sorting.cpp FileManager.cpp Queue.cpp Stack.cpp PriorityQueue.cpp RequestSorting.cpp Activity.cpp BinarySearchTree.cpp HashTable.cpp CampusGraph.cpp
./campus_system
```

### Run the automated tests (optional but impressive in the demo)
```bash
make test                              # 7 unit-test programs
python3 tests/integration_test.py      # 37 end-to-end checks through the real menu (needs Python 3)
```
On Windows: double-click `tests\run_tests.bat`.

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

**Quick demo path (≈5 minutes):** Menu 1→5 (see data) · 1→1 (add a lab) · 1→6 (delete it) · 6→2 (undo restores it) · 4→1 (submit) · 4→3 (process FIFO) · 5→1 ×3 with priorities 4,1,2 then 5→3 ×3 (urgent first) · 2→4 (linear vs binary comparison) · 3→1 (sort by capacity) · 7→1 (traversals) · 8→1 (hash lookup) · 9→5 (shortest route) · 10→2 (benchmarks) · 0 (save).

---

## 5. Data files (`data/`)

| File | One line looks like | Notes |
|------|---------------------|-------|
| `resources.txt` | `LAB101\|ICT Lab\|Computer Laboratory\|Building 32\|40\|Available\|Good` | id, name, type, building, capacity, status, condition |
| `requests.txt` | `5\|Dr Nkosi\|LAB310\|Fume cupboard failure\|maintenance\|1\|Pending\|2026-10-12 09:10:00\|P` | last field: `N` normal queue, `P` priority queue, `H` processed history |
| `locations.txt` | `LOCATION\|Library` and `EDGE\|Library\|Academic Building` | graph vertices and pathways |
| `activities.txt` | `1\|C++ practical test\|LAB101\|2026-11-02\|09:00\|2` | id, name, venue, date, time, priority |

The `|` character separates fields, so it cannot be typed inside a field (the program replaces it with `/`). Files are loaded when the program starts and saved when you choose **0. Exit** (or Menu 1 → 8). If you close the window without choosing Exit, changes are **not** saved.

**Large data test:** `cd data` then `python3 generate_large_data.py 5000` creates `resources_large.txt`. Copy it over `resources.txt` (keep a backup!) and start the program. Menu 10 → 2 also measures speed on generated data of 100 / 1 000 / 5 000 records without touching your files.

---

## 6. Known limitations (be ready to discuss them)

- The priority queue is a **sorted linked list** (insert O(n), remove O(1)), not a binary heap (insert O(log n)). Both are valid priority queues; the report explains the trade-off.
- The BST does **not rebalance**. Inserting IDs in already-sorted order makes it a chain (worst case O(n)). The sample data is deliberately in mixed order. A self-balancing (AVL) tree is a possible extension.
- The hash table has a fixed 101 buckets (separate chaining never "fills up", but chains get longer as the table grows).
- The campus graph is undirected and unweighted: "shortest" means the fewest connections, not metres. Maximum 30 locations.
- Undo covers laboratory add / remove / update and request-caused status changes. It does not reverse queue operations (a processed request stays in the history).
- Sorting menus sort a **copy**; the stored list order is unchanged.
- Console program for one user; no database.

---

## 7. External code and academic integrity

All data structures and algorithms were written by the group members (Students A, B and C). No external libraries were used other than the C++ standard library for input/output, strings and timing. The merge of the three parts into one program is described in `docs/INTEGRATION_NOTES.md`.

We confirm that every member can explain the whole system.
