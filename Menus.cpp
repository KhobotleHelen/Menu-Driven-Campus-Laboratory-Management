// Menus.cpp   (INTEGRATION - combines the menus of Students A, B and C)
// Every menu calls the data structure modules written by the three students.
#include "Menus.h"
#include "InputHelper.h"
#include "Searching.h"
#include "Sorting.h"
#include "RequestSorting.h"
#include "Analysis.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>

using namespace std;

// ===========================================================================
// Small shared helper functions (display, input, undo)
// ===========================================================================

// Converts a number to text. (std::to_string is missing in some older MinGW / Dev-C++ compilers,
// so this small helper is used instead and works everywhere.)
static string toText(int number)
{
    ostringstream stream;
    stream << number;
    return stream.str();
}

// Shows ONE laboratory in full (Student A's displayLaboratory).
static void showLab(const Laboratory& lab)
{
    cout << "\nID: " << lab.getId() << endl;
    cout << "Name: " << lab.getName() << endl;
    cout << "Type: " << lab.getType() << endl;
    cout << "Building: " << lab.getBuilding() << endl;
    cout << "Capacity: " << lab.getCapacity() << endl;
    cout << "Status: " << lab.getStatus() << endl;
    cout << "Condition: " << lab.getCondition() << endl;
}

// Shows an array of laboratories as a compact table.
static void printLabTable(Laboratory* labs, int n)
{
    if (n == 0) {
        cout << "No laboratories to show.\n";
        return;
    }
    cout << left << setw(10) << "ID" << setw(28) << "Name" << setw(22) << "Type"
         << setw(20) << "Building" << setw(9) << "Capacity" << setw(13) << "Status" << "Condition\n";
    cout << string(104, '-') << '\n';
    for (int i = 0; i < n; i++) {
        cout << left << setw(10) << labs[i].getId() << setw(28) << labs[i].getName().substr(0, 26)
             << setw(22) << labs[i].getType().substr(0, 20) << setw(20) << labs[i].getBuilding().substr(0, 18)
             << setw(9) << labs[i].getCapacity() << setw(13) << labs[i].getStatus()
             << labs[i].getCondition() << '\n';
    }
    cout << n << " laboratory record(s).\n";
}

// Shows an array of requests as a compact table.
static void printRequestTable(Request* reqs, int n)
{
    if (n == 0) {
        cout << "No requests to show.\n";
        return;
    }
    cout << left << setw(6) << "ID" << setw(16) << "User" << setw(10) << "Resource" << setw(22) << "Item"
         << setw(13) << "Type" << setw(5) << "Pri" << setw(21) << "Time" << "Status\n";
    cout << string(104, '-') << '\n';
    for (int i = 0; i < n; i++) {
        cout << left << setw(6) << reqs[i].requestId << setw(16) << reqs[i].studentId.substr(0, 14)
             << setw(10) << reqs[i].laboratory << setw(22) << reqs[i].item.substr(0, 20)
             << setw(13) << reqs[i].requestType.substr(0, 11) << setw(5) << reqs[i].priority
             << setw(21) << reqs[i].requestTime << reqs[i].status << '\n';
    }
}

// Lets the user choose a status (keeps the data consistent: always one of three values).
static string chooseStatus()
{
    cout << "Status:  1 = Available   2 = Booked   3 = Maintenance\n";
    int choice = readInt("Choose status (1-3): ", 1, 3);
    if (choice == 1) return "Available";
    if (choice == 2) return "Booked";
    return "Maintenance";
}

// Reads all laboratory details from the keyboard (Student A's enterLaboratory, made safer).
// If fixedId is not empty the ID is not asked for (used when updating).
static Laboratory readLaboratory(const string& fixedId)
{
    string id = fixedId;
    if (id.empty()) id = readNonEmpty("Enter Laboratory ID: ");
    string name = readNonEmpty("Enter Laboratory Name: ");
    string building = readNonEmpty("Enter Building: ");
    string type = readNonEmpty("Enter Laboratory Type: ");
    int capacity = readInt("Enter Capacity (1-10000): ", 1, 10000);
    string status = chooseStatus();
    string condition = readNonEmpty("Enter Condition (e.g. Good): ");
    return Laboratory(id, name, type, building, capacity, status, condition);
}

// Records an UNDO action on the stack.
static void recordUndo(CampusSystem& sys, const string& type, const string& description,
                       const Laboratory& before, const Laboratory& after, int position)
{
    UndoAction action;
    action.type = type;
    action.description = description;
    action.before = before;
    action.after = after;
    action.position = position;
    sys.undoStack.push(action);
}

// Changes only the STATUS of a laboratory and records it for undo.
static void changeStatusWithUndo(CampusSystem& sys, const Laboratory& before, const string& newStatus,
                                 const string& description)
{
    Laboratory after = before;
    after.setStatus(newStatus);
    sys.resources.updateResource(before.getId(), after);
    recordUndo(sys, "UPDATE", description, before, after, 0);
}

// Reverses the most recent action on the undo stack (pop, then do the opposite).
static void performUndo(CampusSystem& sys)
{
    UndoAction action;
    if (!sys.undoStack.pop(action)) {
        cout << "Nothing to undo; the stack is empty.\n";
        return;
    }

    if (action.type == "ADD") {
        // The opposite of ADD is REMOVE.
        Laboratory removed;
        int oldPosition = 0;
        if (sys.resources.removeResource(action.after.getId(), removed, oldPosition))
            cout << "UNDO: removed laboratory " << removed.getId() << " that had been added.\n";
        else
            cout << "UNDO: laboratory " << action.after.getId() << " no longer exists, nothing to remove.\n";
    }
    else if (action.type == "REMOVE") {
        // The opposite of REMOVE is to ADD the saved copy back at its old position.
        int code = sys.resources.addResource(action.before, ADD_AT_POSITION, action.position);
        if (code == ADD_BAD_POSITION)
            code = sys.resources.addResource(action.before, ADD_AT_END);
        if (code == ADD_OK)
            cout << "UNDO: restored laboratory " << action.before.getId() << " (deleted earlier).\n";
        else
            cout << "UNDO failed: a laboratory with ID " << action.before.getId() << " already exists.\n";
    }
    else if (action.type == "UPDATE") {
        // The opposite of UPDATE is to put the old data back.
        if (sys.resources.updateResource(action.before.getId(), action.before))
            cout << "UNDO: restored the previous details of laboratory " << action.before.getId() << ".\n";
        else
            cout << "UNDO failed: laboratory " << action.before.getId() << " no longer exists.\n";
    }
    else {
        cout << "UNDO: removed note \"" << action.description << "\" (a note changes no data).\n";
    }
}

// Builds a default campus graph when no locations file exists (Student C's initializeGraph).
static void seedDefaultGraph(CampusGraph& graph)
{
    graph.addLocation("Gate");
    graph.addLocation("Library");
    graph.addLocation("Lab A");
    graph.addLocation("Hall");
    graph.addLocation("Lab B");

    graph.addConnection("Gate", "Library");
    graph.addConnection("Library", "Lab A");
    graph.addConnection("Gate", "Hall");
    graph.addConnection("Hall", "Lab B");
    graph.addConnection("Lab B", "Library");
}

// ===========================================================================
// Loading and saving everything (Module 10)
// ===========================================================================

void loadAllData(CampusSystem& sys)
{
    // ---- Start from a clean state ----
    sys.resources.clearAll();
    Request r;
    while (sys.normalQueue.dequeue(r)) {}
    while (sys.priorityQueue.processHighestPriority(r)) {}
    while (sys.processedLog.dequeue(r)) {}
    UndoAction u;
    while (sys.undoStack.pop(u)) {}
    sys.nextRequestId = 1;

    // ---- Laboratories: file -> temporary list -> add one by one (this validates every record) ----
    LinkedList temp;
    if (sys.files.loadFromFile(temp, RESOURCE_FILE)) {
        int n = 0;
        Laboratory* labs = temp.toArray(n);
        int skipped = 0;
        for (int i = 0; i < n; i++) {
            if (sys.resources.addResource(labs[i], ADD_AT_END) != ADD_OK) skipped++;
            // Empty or duplicate IDs in the file are skipped.
        }
        delete[] labs;
        cout << "Loaded " << sys.resources.count() << " laboratories";
        if (skipped > 0) cout << " (" << skipped << " invalid/duplicate record(s) skipped)";
        cout << ".\n";
    } else {
        cout << "No resources file found (" << RESOURCE_FILE << "). Starting with no laboratories.\n";
    }

    // ---- Requests ----
    if (sys.files.loadRequests(sys.normalQueue, sys.priorityQueue, sys.processedLog, REQUEST_FILE, sys.nextRequestId)) {
        cout << "Loaded requests: " << sys.normalQueue.size() << " normal, " << sys.priorityQueue.size()
             << " priority, " << sys.processedLog.size() << " processed.\n";
    } else {
        cout << "No requests file found. Starting with no requests.\n";
    }

    // ---- Campus graph ----
    if (sys.files.loadLocations(sys.graph, LOCATION_FILE) && sys.graph.locationCount() > 0) {
        cout << "Loaded " << sys.graph.locationCount() << " campus locations.\n";
    } else {
        sys.graph.clear();
        seedDefaultGraph(sys.graph);
        cout << "No locations file found. Using the built-in sample campus map.\n";
    }

    // ---- Activities ----
    if (sys.files.loadActivities(sys.activities, ACTIVITY_FILE)) {
        cout << "Loaded " << sys.activities.size() << " scheduled activities.\n";
    } else {
        cout << "No activities file found. Starting with an empty timetable.\n";
    }
}

void saveAllData(CampusSystem& sys)
{
    bool ok = true;
    ok = sys.files.saveToFile(sys.resources.list, RESOURCE_FILE) && ok;
    ok = sys.files.saveRequests(sys.normalQueue, sys.priorityQueue, sys.processedLog, REQUEST_FILE) && ok;
    ok = sys.files.saveLocations(sys.graph, LOCATION_FILE) && ok;
    ok = sys.files.saveActivities(sys.activities, ACTIVITY_FILE) && ok;
    if (ok) cout << "All data saved to the data folder.\n";
    else    cout << "WARNING: some data could not be saved.\n";
}

// ===========================================================================
// MENU 1: RESOURCE MANAGEMENT  (Student A - linked list operations)
// ===========================================================================

// Adds a laboratory at the front / end / position and records UNDO information.
static void addLaboratoryInteractive(CampusSystem& sys, int where)
{
    int position = 0;
    if (where == ADD_AT_POSITION) {
        cout << "The list currently has " << sys.resources.list.size() << " laboratories.\n";
        position = readInt("Enter position (1 = first): ", 1, sys.resources.list.size() + 1);
    }

    string id = readNonEmpty("Enter Laboratory ID: ");
    if (sys.resources.table.search(id) != NULL) {
        cout << "A laboratory with ID " << id << " already exists. IDs must be unique.\n";
        return;
    }
    Laboratory lab = readLaboratory(id);

    int code = sys.resources.addResource(lab, where, position);
    if (code == ADD_OK) {
        cout << "\nLaboratory added successfully.\n";
        recordUndo(sys, "ADD", "Added laboratory " + lab.getId(), Laboratory(), lab, 0);
    } else if (code == ADD_DUPLICATE) {
        cout << "Duplicate ID rejected.\n";
    } else if (code == ADD_BAD_POSITION) {
        cout << "That position does not exist.\n";
    } else {
        cout << "Laboratory could not be added.\n";
    }
}

void resourceMenu(CampusSystem& sys)
{
    int choice;
    do {
        cout << "\n----- 1. RESOURCE MANAGEMENT -----\n";
        cout << "1. Add laboratory at beginning\n";
        cout << "2. Add laboratory at end\n";
        cout << "3. Add laboratory at a position\n";
        cout << "4. Display all laboratories (detailed linked-list traversal)\n";
        cout << "5. Display all laboratories (compact table)\n";
        cout << "6. Remove laboratory\n";
        cout << "7. Update laboratory\n";
        cout << "8. Save all data now\n";
        cout << "9. Reload all data from files (discards unsaved changes)\n";
        cout << "0. Back to main menu\n";
        choice = readInt("Choose an option: ", 0, 9);

        if (choice == 1) {
            addLaboratoryInteractive(sys, ADD_AT_FRONT);
        }
        else if (choice == 2) {
            addLaboratoryInteractive(sys, ADD_AT_END);
        }
        else if (choice == 3) {
            addLaboratoryInteractive(sys, ADD_AT_POSITION);
        }
        else if (choice == 4) {
            cout << "\n===== LABORATORIES =====" << endl;
            sys.resources.list.display();
            // Traverses the linked list from the head to the end.
        }
        else if (choice == 5) {
            int n = 0;
            Laboratory* labs = sys.resources.list.toArray(n);
            printLabTable(labs, n);
            delete[] labs;
        }
        else if (choice == 6) {
            string id = readNonEmpty("Enter Laboratory ID to remove: ");
            Laboratory removed;
            int oldPosition = 0;
            if (sys.resources.removeResource(id, removed, oldPosition)) {
                cout << "\nLaboratory removed successfully (it was at position " << oldPosition << ").\n";
                recordUndo(sys, "REMOVE", "Removed laboratory " + removed.getId(), removed, Laboratory(), oldPosition);
            } else {
                cout << "\nLaboratory not found.\n";
            }
        }
        else if (choice == 7) {
            string id = readNonEmpty("Enter Laboratory ID to update: ");
            Laboratory* found = sys.resources.list.searchById(id);
            if (found == NULL) {
                cout << "\nLaboratory not found.\n";
            } else {
                Laboratory before = *found;
                cout << "\nCurrent details:";
                showLab(before);
                cout << "\nEnter the new details (the ID cannot be changed):\n";
                Laboratory updated = readLaboratory(id);
                if (sys.resources.updateResource(id, updated)) {
                    cout << "\nLaboratory updated successfully.\n";
                    recordUndo(sys, "UPDATE", "Updated laboratory " + id, before, updated, 0);
                }
            }
        }
        else if (choice == 8) {
            saveAllData(sys);
        }
        else if (choice == 9) {
            loadAllData(sys);
        }
    } while (choice != 0);
}

// ===========================================================================
// MENU 2: SEARCH  (Student A - linear and binary search)
// ===========================================================================

void searchMenu(CampusSystem& sys)
{
    int choice;
    do {
        cout << "\n----- 2. SEARCH RESOURCES -----\n";
        cout << "1. Linear search by ID\n";
        cout << "2. Linear search by name\n";
        cout << "3. Binary search by ID (data is sorted first)\n";
        cout << "4. Compare linear and binary search for one ID\n";
        cout << "0. Back to main menu\n";
        choice = readInt("Choose an option: ", 0, 4);
        if (choice == 0) break;

        int n = 0;
        Laboratory* labs = sys.resources.list.toArray(n);
        // Copy the linked list into an array, because both searches work on arrays.

        if (n == 0) {
            cout << "\nThere are no laboratories to search.\n";
            delete[] labs;
            continue;
        }

        if (choice == 1) {
            string id = readNonEmpty("Enter Laboratory ID to search: ");
            int steps = 0;
            Laboratory* result = linearSearch(labs, n, id, &steps);
            if (result != NULL) { cout << "\nLaboratory found:"; showLab(*result); }
            else cout << "\nLaboratory not found.\n";
            cout << "Linear search made " << steps << " comparison(s) among " << n << " laboratories.\n";
        }
        else if (choice == 2) {
            string name = readNonEmpty("Enter Laboratory name to search: ");
            int steps = 0;
            Laboratory* result = linearSearchByName(labs, n, name, &steps);
            if (result != NULL) { cout << "\nLaboratory found:"; showLab(*result); }
            else cout << "\nLaboratory not found.\n";
            cout << "Linear search made " << steps << " comparison(s) among " << n << " laboratories.\n";
        }
        else if (choice == 3 || choice == 4) {
            string id = readNonEmpty("Enter Laboratory ID to search: ");
            int linearSteps = 0;
            if (choice == 4) linearSearch(labs, n, id, &linearSteps);
            // For the comparison we first run the linear search on the UNSORTED data.

            mergeSortById(labs, 0, n - 1);
            // Binary search REQUIRES sorted data, so sort the array copy by ID first.

            int steps = 0;
            int position = binarySearch(labs, n, id, &steps);
            if (position != -1) { cout << "\nLaboratory found:"; showLab(labs[position]); }
            else cout << "\nLaboratory not found.\n";
            cout << "Binary search made " << steps << " comparison(s) among " << n << " laboratories.\n";
            if (choice == 4) {
                cout << "Linear search made " << linearSteps << " comparison(s) for the same ID.\n";
                cout << "Linear is O(n); binary is O(log n) but needs the data to be sorted first.\n";
            }
        }
        delete[] labs;
        // Free the temporary array.
    } while (choice != 0);
}

// ===========================================================================
// MENU 3: SORT  (Student A for resources, Student B for requests)
// ===========================================================================

// Collects the requests of BOTH queues into one new array (caller must delete[]).
static Request* gatherPendingRequests(CampusSystem& sys, int& total)
{
    int n1 = 0, n2 = 0;
    Request* a = sys.normalQueue.toArray(n1);
    Request* b = sys.priorityQueue.toArray(n2);
    total = n1 + n2;
    if (total == 0) {
        delete[] a;
        delete[] b;
        return NULL;
    }
    Request* all = new Request[total];
    for (int i = 0; i < n1; i++) all[i] = a[i];
    for (int i = 0; i < n2; i++) all[n1 + i] = b[i];
    delete[] a;
    delete[] b;
    return all;
}

void sortMenu(CampusSystem& sys)
{
    int choice;
    do {
        cout << "\n----- 3. SORT -----\n";
        cout << "1. Sort laboratories\n";
        cout << "2. Sort pending requests\n";
        cout << "0. Back to main menu\n";
        choice = readInt("Choose an option: ", 0, 2);

        if (choice == 1) {
            int n = 0;
            Laboratory* labs = sys.resources.list.toArray(n);
            if (n == 0) {
                cout << "\nThere are no laboratories to sort.\n";
                delete[] labs;
                continue;
            }
            cout << "Sort by:  1 = ID   2 = Name   3 = Capacity   4 = Type\n";
            int key = readInt("Choose key (1-4): ", 1, 4);
            cout << "Algorithm:  1 = Bubble sort O(n^2)   2 = Merge sort O(n log n)\n";
            int algorithm = readInt("Choose algorithm (1-2): ", 1, 2);

            chrono::steady_clock::time_point start = chrono::steady_clock::now();
            if (algorithm == 1) bubbleSortBy(labs, n, key);
            else mergeSortBy(labs, 0, n - 1, key);
            chrono::steady_clock::time_point stop = chrono::steady_clock::now();
            // The sort works on a COPY, so the stored data is untouched.

            cout << "\nSorted laboratories:\n";
            printLabTable(labs, n);
            cout << (algorithm == 1 ? "Bubble sort" : "Merge sort") << " took "
                 << chrono::duration_cast<chrono::microseconds>(stop - start).count() << " microseconds.\n";
            delete[] labs;
        }
        else if (choice == 2) {
            int n = 0;
            Request* reqs = gatherPendingRequests(sys, n);
            if (n == 0) {
                cout << "\nThere are no pending requests to sort.\n";
                continue;
            }
            cout << "Sort by:  1 = Submission time   2 = Priority   3 = Status\n";
            int key = readInt("Choose key (1-3): ", 1, 3);
            cout << "Algorithm:  1 = Insertion sort O(n^2)   2 = Merge sort O(n log n)\n";
            int algorithm = readInt("Choose algorithm (1-2): ", 1, 2);

            if (algorithm == 1) insertionSortRequests(reqs, n, key);
            else mergeSortRequests(reqs, 0, n - 1, key);

            cout << "\nSorted pending requests (normal + priority):\n";
            printRequestTable(reqs, n);
            delete[] reqs;
        }
    } while (choice != 0);
}

// ===========================================================================
// REQUEST HANDLING (Student B) - shared by menus 4 and 5
// ===========================================================================

// Asks for the details of a new request. Returns false if the user gave an unknown resource.
static bool readRequest(CampusSystem& sys, bool priorityRequest, Request& result)
{
    string student = readNonEmpty("User (student ID or name): ");
    string labId = readNonEmpty("Requested resource ID (e.g. LAB101): ");

    const Laboratory* lab = sys.resources.table.search(labId);
    // Hash table lookup: is this a real resource? (average O(1))

    if (lab == NULL) {
        cout << "Resource " << labId << " does not exist. Request cancelled.\n";
        return false;
    }
    cout << "Resource found: " << lab->getName() << " (" << lab->getStatus() << ")\n";

    string item = readNonEmpty("Purpose / equipment needed: ");

    cout << "Request type:  1 = booking   2 = equipment   3 = maintenance   4 = access\n";
    int t = readInt("Choose type (1-4): ", 1, 4);
    string type = (t == 1 ? "booking" : t == 2 ? "equipment" : t == 3 ? "maintenance" : "access");

    int priority = 3;
    if (priorityRequest) {
        cout << "Priority levels: 1=Emergency, 2=High, 3=Normal, 4=Low\n";
        priority = readInt("Enter priority (1-4): ", 1, 4);
    }

    result = Request(sys.nextRequestId++, student, labId, item, type, priority);
    return true;
}

// Processes one request that has just been removed from a queue:
//   - checks the resource through the hash table,
//   - approves (and books / marks maintenance) or rejects,
//   - stores the result in the processed history.
static void processRequest(CampusSystem& sys, Request req)
{
    const Laboratory* lab = sys.resources.table.search(req.laboratory);
    string outcome;

    if (lab == NULL) {
        req.status = "Rejected";
        outcome = "the resource no longer exists";
    } else {
        Laboratory before = *lab;
        // Copy first: the stored object is replaced when we update it.

        if (toLower(req.requestType) == "maintenance") {
            if (before.getStatus() == "Maintenance") {
                req.status = "Approved";
                outcome = "the resource is already under maintenance";
            } else {
                changeStatusWithUndo(sys, before, "Maintenance", "Request #" + toText(req.requestId) + ": " + before.getId() + " set to Maintenance");
                req.status = "Approved";
                outcome = "resource set to Maintenance";
            }
        } else if (before.getStatus() == "Available") {
            changeStatusWithUndo(sys, before, "Booked", "Request #" + toText(req.requestId) + ": " + before.getId() + " booked");
            req.status = "Approved";
            outcome = "resource is now Booked";
        } else {
            req.status = "Rejected";
            outcome = "the resource is currently " + before.getStatus();
        }
    }

    sys.processedLog.enqueue(req, false);
    // Remember the processed request in the history.

    cout << "Processed request #" << req.requestId << " from " << req.studentId
         << " (priority " << req.priority << ") for " << req.laboratory
         << ": " << req.status << " - " << outcome << ".\n";
}

// ===========================================================================
// MENU 4: REQUESTS (FIFO queue) AND ACTIVITIES  (Student B)
// ===========================================================================

void requestMenu(CampusSystem& sys)
{
    int choice;
    do {
        cout << "\n----- 4. RESOURCE REQUESTS (normal, first-come first-served) -----\n";
        cout << "1. Submit a normal request (enqueue)\n";
        cout << "2. View pending normal requests\n";
        cout << "3. Process next normal request (dequeue - FIFO)\n";
        cout << "4. Peek at the next request without removing it\n";
        cout << "5. View processed request history\n";
        cout << "6. Schedule a laboratory activity\n";
        cout << "7. View the activity timetable\n";
        cout << "0. Back to main menu\n";
        choice = readInt("Choose an option: ", 0, 7);

        if (choice == 1) {
            Request request;
            if (readRequest(sys, false, request)) {
                sys.normalQueue.enqueue(request);
            }
        }
        else if (choice == 2) {
            sys.normalQueue.display();
        }
        else if (choice == 3) {
            Request request;
            if (sys.normalQueue.dequeue(request)) {
                processRequest(sys, request);
            } else {
                cout << "No normal requests are waiting.\n";
            }
        }
        else if (choice == 4) {
            Request request;
            if (sys.normalQueue.peekFront(request)) {
                cout << "Next to be served: request #" << request.requestId << " from " << request.studentId
                     << " for " << request.laboratory << " (" << request.item << ").\n";
            } else {
                cout << "The normal queue is empty.\n";
            }
        }
        else if (choice == 5) {
            sys.processedLog.display("Processed Requests (oldest first)");
        }
        else if (choice == 6) {
            Activity a;
            a.name = readNonEmpty("Activity name: ");
            string venue = readNonEmpty("Laboratory/venue ID: ");
            const Laboratory* lab = sys.resources.table.search(venue);
            if (lab == NULL) {
                cout << "Venue " << venue << " does not exist. Activity not scheduled.\n";
                continue;
            }
            if (lab->getStatus() == "Maintenance") {
                cout << "Warning: " << venue << " is currently under maintenance.\n";
            }
            a.venue = venue;
            do {
                a.date = readNonEmpty("Date (YYYY-MM-DD, e.g. 2026-11-02): ");
                if (!ActivityScheduler::validDate(a.date)) cout << "Invalid date format.\n";
            } while (!ActivityScheduler::validDate(a.date));
            do {
                a.time = readNonEmpty("Time (HH:MM, 24 hour, e.g. 10:00): ");
                if (!ActivityScheduler::validTime(a.time)) cout << "Invalid time format.\n";
            } while (!ActivityScheduler::validTime(a.time));
            a.priority = readInt("Priority (1=Emergency, 2=High, 3=Normal, 4=Low): ", 1, 4);
            sys.activities.addActivity(a);
        }
        else if (choice == 7) {
            sys.activities.display();
        }
    } while (choice != 0);
}

// ===========================================================================
// MENU 5: PRIORITY REQUESTS  (Student B)
// ===========================================================================

void priorityMenu(CampusSystem& sys)
{
    int choice;
    do {
        cout << "\n----- 5. PRIORITY REQUESTS -----\n";
        cout << "1. Submit a priority request (insert)\n";
        cout << "2. View priority requests (most urgent first)\n";
        cout << "3. Process the highest-priority request (remove)\n";
        cout << "4. Peek at the highest-priority request\n";
        cout << "5. Explain how the priority queue works\n";
        cout << "0. Back to main menu\n";
        choice = readInt("Choose an option: ", 0, 5);

        if (choice == 1) {
            Request request;
            if (readRequest(sys, true, request)) {
                sys.priorityQueue.insert(request);
            }
        }
        else if (choice == 2) {
            sys.priorityQueue.display();
        }
        else if (choice == 3) {
            Request request;
            if (sys.priorityQueue.processHighestPriority(request)) {
                processRequest(sys, request);
            } else {
                cout << "No priority requests are waiting.\n";
            }
        }
        else if (choice == 4) {
            Request request;
            if (sys.priorityQueue.peekHighest(request)) {
                cout << "Most urgent: request #" << request.requestId << " (priority " << request.priority
                     << ") from " << request.studentId << " for " << request.laboratory << ".\n";
            } else {
                cout << "The priority queue is empty.\n";
            }
        }
        else if (choice == 5) {
            cout << "\nA PRIORITY QUEUE is an abstract data type: items leave in order of importance, not arrival.\n"
                 << "A HEAP is one way to implement it (a tree stored in an array whose root is always the most urgent item;\n"
                 << "insert and remove are O(log n)).\n"
                 << "This project implements the priority queue as a LINKED LIST kept sorted by priority:\n"
                 << "  - the most urgent request is always at the head, so removal is O(1);\n"
                 << "  - insertion walks down to the right place, so it is O(n);\n"
                 << "  - requests with the same priority stay in arrival order (FIFO among equals).\n"
                 << "Levels: 1 = Emergency, 2 = High, 3 = Normal, 4 = Low.\n";
        }
    } while (choice != 0);
}

// ===========================================================================
// MENU 6: UNDO  (Student B - stack)
// ===========================================================================

void undoMenu(CampusSystem& sys)
{
    int choice;
    do {
        cout << "\n----- 6. UNDO OPERATIONS (stack) -----\n";
        cout << "1. View most recent action (peek / top)\n";
        cout << "2. Undo the most recent action (pop)\n";
        cout << "3. Record a manual note (push)\n";
        cout << "4. Show stack status (empty? size)\n";
        cout << "0. Back to main menu\n";
        choice = readInt("Choose an option: ", 0, 4);

        if (choice == 1) {
            sys.undoStack.displayTop();
        }
        else if (choice == 2) {
            performUndo(sys);
        }
        else if (choice == 3) {
            string note = readNonEmpty("Describe the laboratory action: ");
            sys.undoStack.push(note);
        }
        else if (choice == 4) {
            cout << "Undo stack is " << (sys.undoStack.isEmpty() ? "EMPTY" : "not empty")
                 << ". Actions stored: " << sys.undoStack.size() << ".\n";
        }
    } while (choice != 0);
}

// ===========================================================================
// MENU 7: TREE  (Student C - binary search tree)
// ===========================================================================

void treeMenu(CampusSystem& sys)
{
    int choice;
    do {
        cout << "\n----- 7. TREE OPERATIONS (Binary Search Tree by laboratory ID) -----\n";
        cout << "1. Display the three traversals\n";
        cout << "2. Search the tree by ID\n";
        cout << "3. Tree statistics\n";
        cout << "0. Back to main menu\n";
        choice = readInt("Choose an option: ", 0, 3);

        if (choice == 1) {
            if (sys.resources.bst.isEmpty()) {
                cout << "The tree is empty.\n";
            } else {
                cout << "\nInorder (left, node, right - sorted order): ";
                sys.resources.bst.inorder();
                cout << "Preorder (node, left, right): ";
                sys.resources.bst.preorder();
                cout << "Postorder (left, right, node): ";
                sys.resources.bst.postorder();
            }
        }
        else if (choice == 2) {
            string id = readNonEmpty("Enter Laboratory ID: ");
            const Laboratory* lab = sys.resources.bst.search(id);
            if (lab != NULL) { cout << "\nFound in the tree:"; showLab(*lab); }
            else cout << "Laboratory not found in the tree.\n";
        }
        else if (choice == 3) {
            int n = sys.resources.bst.size();
            int h = sys.resources.bst.height();
            int ideal = 0;
            for (int capacity = 1; capacity - 1 < n; capacity *= 2) ideal++;
            // Smallest height a perfectly balanced tree of n nodes could have: ceil(log2(n+1)).

            cout << "Nodes: " << n << "\nHeight: " << h << "\nIdeal (perfectly balanced) height: " << ideal << "\n";
            if (h > 2 * ideal + 1) cout << "The tree is quite unbalanced, so searches are slower than O(log n).\n";
        }
    } while (choice != 0);
}

// ===========================================================================
// MENU 8: HASH TABLE  (Student C)
// ===========================================================================

void hashMenu(CampusSystem& sys)
{
    int choice;
    do {
        cout << "\n----- 8. HASH TABLE LOOKUP (Resource ID -> Laboratory) -----\n";
        cout << "1. Look up a laboratory by ID\n";
        cout << "2. Display the hash table buckets\n";
        cout << "3. Hash table statistics\n";
        cout << "0. Back to main menu\n";
        choice = readInt("Choose an option: ", 0, 3);

        if (choice == 1) {
            string id = readNonEmpty("Enter Laboratory ID: ");
            const Laboratory* lab = sys.resources.table.search(id);
            cout << "The ID \"" << id << "\" hashes to bucket " << sys.resources.table.getIndex(id) << ".\n";
            if (lab != NULL) { cout << "Found:"; showLab(*lab); }
            else cout << "Laboratory not found.\n";
        }
        else if (choice == 2) {
            if (sys.resources.table.size() == 0) cout << "The hash table is empty.\n";
            else sys.resources.table.display();
        }
        else if (choice == 3) {
            sys.resources.table.displayStats();
        }
    } while (choice != 0);
}

// ===========================================================================
// MENU 9: CAMPUS NAVIGATION  (Student C - graph)
// ===========================================================================

void navigationMenu(CampusSystem& sys)
{
    CampusGraph& graph = sys.graph;
    // A short name for the graph inside this menu.

    int choice;
    do {
        cout << "\n===== 9. CAMPUS NAVIGATION (graph) =====\n";
        cout << "1. Display graph\n";
        cout << "2. BFS traversal\n";
        cout << "3. DFS traversal\n";
        cout << "4. Check connectivity\n";
        cout << "5. Shortest path\n";
        cout << "6. Add location\n";
        cout << "7. Add connection\n";
        cout << "8. Count connected components\n";
        cout << "9. Explain BFS versus DFS\n";
        cout << "0. Back\n";
        choice = readInt("Choose option: ", 0, 9);

        if (choice == 1) {
            graph.display();
        }
        else if (choice == 2) {
            string start = readNonEmpty("Start location: ");
            graph.BFS(start);
        }
        else if (choice == 3) {
            string start = readNonEmpty("Start location: ");
            graph.DFS(start);
        }
        else if (choice == 4 || choice == 5) {
            string from = readNonEmpty("From: ");
            string to = readNonEmpty("To: ");

            int distance = graph.shortestPath(from, to);

            if (choice == 4) {
                cout << (distance >= 0
                    ? "Locations are connected.\n"
                    : "Locations are not connected or do not exist.\n");
            }
            else if (distance >= 0) {
                graph.printShortestRoute(from, to);
                cout << "Minimum connections: " << distance << '\n';
            }
            else {
                cout << "No route found.\n";
            }
        }
        else if (choice == 6) {
            string name = readNonEmpty("Location name: ");
            cout << (graph.addLocation(name) ? "Location added.\n" : "Cannot add location (duplicate, or the map is full).\n");
        }
        else if (choice == 7) {
            string from = readNonEmpty("From: ");
            string to = readNonEmpty("To: ");
            cout << (graph.addConnection(from, to) ? "Connection added.\n" : "Cannot add connection (unknown location, same location, or already connected).\n");
        }
        else if (choice == 8) {
            cout << "The campus map has " << graph.connectedComponents() << " connected component(s) "
                 << "(1 means every location can be reached from every other).\n";
        }
        else if (choice == 9) {
            cout << "\nBFS explores level by level (all neighbours first) using a QUEUE; it finds the route with the\n"
                 << "fewest connections. DFS goes as deep as possible along one path before backing up (recursion / stack);\n"
                 << "it is good for exploring everything reachable and for detecting components. Both are O(V + E).\n";
        }
    } while (choice != 0);
}

// ===========================================================================
// MENU 10: SYSTEM ANALYSIS
// ===========================================================================

void analysisMenu(CampusSystem& sys)
{
    int choice;
    do {
        cout << "\n----- 10. SYSTEM ANALYSIS -----\n";
        cout << "1. Show complexity analysis table\n";
        cout << "2. Run performance measurements (generated data)\n";
        cout << "3. Show current system statistics\n";
        cout << "0. Back to main menu\n";
        choice = readInt("Choose an option: ", 0, 3);

        if (choice == 1) {
            showComplexityTable();
        }
        else if (choice == 2) {
            runBenchmarks();
        }
        else if (choice == 3) {
            cout << "\nLaboratories (linked list)      : " << sys.resources.list.size() << '\n';
            cout << "Laboratories (BST / hash table)  : " << sys.resources.bst.size() << " / " << sys.resources.table.size() << '\n';
            cout << "BST height                       : " << sys.resources.bst.height() << '\n';
            cout << "Normal requests waiting          : " << sys.normalQueue.size() << '\n';
            cout << "Priority requests waiting        : " << sys.priorityQueue.size() << '\n';
            cout << "Processed requests               : " << sys.processedLog.size() << '\n';
            cout << "Undo stack size                  : " << sys.undoStack.size() << '\n';
            cout << "Scheduled activities             : " << sys.activities.size() << '\n';
            cout << "Campus locations                 : " << sys.graph.locationCount() << '\n';
            cout << "Connected components (map)       : " << sys.graph.connectedComponents() << '\n';
            cout << "\nHash table statistics:\n";
            sys.resources.table.displayStats();
        }
    } while (choice != 0);
}
