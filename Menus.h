// Menus.h   (INTEGRATION)
// All menu screens of the system. The numbering follows the "Minimum System Menu" of the brief (Section 28).
#ifndef MENUS_H
#define MENUS_H

#include "CampusSystem.h"

void loadAllData(CampusSystem& sys);      // read every data file at start-up
void saveAllData(CampusSystem& sys);      // write every data file at exit

void resourceMenu(CampusSystem& sys);     // 1. Resource Management   (linked list, CRUD)
void searchMenu(CampusSystem& sys);       // 2. Search Resources      (linear + binary search)
void sortMenu(CampusSystem& sys);         // 3. Sort Resources        (bubble + merge, insertion + merge for requests)
void requestMenu(CampusSystem& sys);      // 4. Resource Requests     (FIFO queue, processing, activities)
void priorityMenu(CampusSystem& sys);     // 5. Priority Requests     (priority queue)
void undoMenu(CampusSystem& sys);         // 6. Undo Operations       (stack)
void treeMenu(CampusSystem& sys);         // 7. Tree Operations       (BST)
void hashMenu(CampusSystem& sys);         // 8. Hash Table Lookup
void navigationMenu(CampusSystem& sys);   // 9. Campus Navigation     (graph)
void analysisMenu(CampusSystem& sys);     // 10. System Analysis      (complexity + benchmarks)

#endif
