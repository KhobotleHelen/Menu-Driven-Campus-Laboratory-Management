// main.cpp   (INTEGRATION - program entry point)
// NUL Smart Laboratory Management System - CS3400 Data Structures and Algorithms mini project.
//
// This file only does three things:
//   1. creates the CampusSystem object that holds every data structure,
//   2. loads the saved data,
//   3. shows the main menu until the user chooses Exit, then saves everything.
// All the real work happens in the modules written by Students A, B and C.
#include <iostream>
#include "CampusSystem.h"
#include "Menus.h"
#include "InputHelper.h"

using namespace std;

// Prints the main menu (the 10 options required by Section 28 of the project brief).
static void showMainMenu()
{
    cout << "\n================================================\n";
    cout << "  CS3400 CAMPUS RESOURCE MANAGEMENT SYSTEM\n";
    cout << "  NUL SMART LABORATORY MANAGEMENT SYSTEM\n";
    cout << "================================================\n";
    cout << " 1. Resource Management\n";
    cout << " 2. Search Resources\n";
    cout << " 3. Sort Resources\n";
    cout << " 4. Resource Requests\n";
    cout << " 5. Priority Requests\n";
    cout << " 6. Undo Operations\n";
    cout << " 7. Tree Operations\n";
    cout << " 8. Hash Table Lookup\n";
    cout << " 9. Campus Navigation\n";
    cout << "10. System Analysis\n";
    cout << " 0. Exit\n";
}

int main()
{
    CampusSystem sys;
    // The whole system: linked list, BST, hash table, queues, stack, graph, activities.

    cout << "Starting system... loading saved data.\n";
    loadAllData(sys);
    // Load the data files when the program starts (Module 10).

    int choice;
    do {
        showMainMenu();
        choice = readInt("Enter your choice: ", 0, 10);
        // readInt keeps asking until a valid number from 0 to 10 is typed.

        switch (choice) {
            case 1:  resourceMenu(sys);   break;
            case 2:  searchMenu(sys);     break;
            case 3:  sortMenu(sys);       break;
            case 4:  requestMenu(sys);    break;
            case 5:  priorityMenu(sys);   break;
            case 6:  undoMenu(sys);       break;
            case 7:  treeMenu(sys);       break;
            case 8:  hashMenu(sys);       break;
            case 9:  navigationMenu(sys); break;
            case 10: analysisMenu(sys);   break;
            case 0:
                saveAllData(sys);
                // Save all changes when the program terminates (Module 10).

                cout << "Data saved. Goodbye!\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
