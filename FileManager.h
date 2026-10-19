// FileManager.h   (Student A - Data Persistence, Module 10)
// Saves data to text files when the program ends and loads it when the program starts.
// Student A wrote the laboratory part; the request / location / activity parts were added during integration
// so that ALL the data files named in the brief (resources, requests, locations) are supported.
//
// File formats (one record per line, fields separated by the | character):
//   resources.txt  : id|name|type|building|capacity|status|condition
//   requests.txt   : requestId|user|resourceId|item|type|priority|status|time|queue    (queue: N=normal, P=priority, H=history)
//   locations.txt  : LOCATION|name      and      EDGE|nameA|nameB
//   activities.txt : id|name|venue|date|time|priority
#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include "LinkedList.h"
#include "Queue.h"
#include "PriorityQueue.h"
#include "CampusGraph.h"
#include "Activity.h"
#include <string>
using namespace std;

class FileManager
{
public:

    // Saves all laboratories from the Linked List into the text file.
    bool saveToFile(LinkedList& list, string filename);

    // Loads laboratories from the text file into the Linked List.
    bool loadFromFile(LinkedList& list, string filename);

    // Saves the normal queue, the priority queue and the processed history to one file.
    bool saveRequests(const RequestQueue& normal, const PriorityRequestQueue& priority,
                      const RequestQueue& history, string filename);

    // Loads requests back into the three structures. nextRequestId becomes (largest ID found + 1).
    bool loadRequests(RequestQueue& normal, PriorityRequestQueue& priority,
                      RequestQueue& history, string filename, int& nextRequestId);

    // Saves / loads the campus graph.
    bool saveLocations(const CampusGraph& graph, string filename);
    bool loadLocations(CampusGraph& graph, string filename);

    // Saves / loads the activity timetable.
    bool saveActivities(const ActivityScheduler& scheduler, string filename);
    bool loadActivities(ActivityScheduler& scheduler, string filename);
};

#endif
