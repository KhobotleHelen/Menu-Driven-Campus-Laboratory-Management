// CampusSystem.h   (INTEGRATION)
// ONE object that holds every data structure of the project, so each menu function receives the
// whole system as a single parameter.
//   Student A : resources.list (linked list), file manager
//   Student B : normalQueue, priorityQueue, processedLog, undoStack, activities
//   Student C : resources.bst, resources.table (hash), graph
#ifndef CAMPUS_SYSTEM_H
#define CAMPUS_SYSTEM_H

#include "ResourceManager.h"
#include "Queue.h"
#include "PriorityQueue.h"
#include "Stack.h"
#include "CampusGraph.h"
#include "Activity.h"
#include "FileManager.h"
#include <string>

// Data files (relative paths: the program must be started from the project folder).
const std::string RESOURCE_FILE   = "data/resources.txt";
const std::string REQUEST_FILE    = "data/requests.txt";
const std::string LOCATION_FILE   = "data/locations.txt";
const std::string ACTIVITY_FILE   = "data/activities.txt";

struct CampusSystem {
    ResourceManager resources;          // linked list + BST + hash table of laboratories
    RequestQueue normalQueue;           // FIFO queue of normal requests
    PriorityRequestQueue priorityQueue; // priority queue of urgent requests
    RequestQueue processedLog;          // history of processed requests (a second queue, oldest first)
    UndoStack undoStack;                // stack for UNDO
    CampusGraph graph;                  // campus locations and pathways
    ActivityScheduler activities;       // timetable of laboratory activities
    FileManager files;                  // saves / loads everything
    int nextRequestId;                  // the number the next new request will get

    CampusSystem() : nextRequestId(1) {}
    // Constructor: request numbering starts at 1.
};

#endif
