// FileManager.cpp   (Student A - Data Persistence)
#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

// ---------------------------------------------------------------------------
// Small helpers used by the loaders (added during integration)
// ---------------------------------------------------------------------------

// Removes a trailing '\r' (files edited on Windows end each line with \r\n).
static void stripCarriageReturn(string& line)
{
    if (!line.empty() && line[line.size() - 1] == '\r')
    {
        line.erase(line.size() - 1);
        // Delete the last character.
    }
}

// Splits "a|b|c" into parts[0]="a", parts[1]="b", parts[2]="c".
// Returns how many fields were found. The LAST field may contain anything (it takes the rest of the line).
static int splitFields(const string& line, string parts[], int maxParts)
{
    int count = 0;
    // Number of fields found so far.

    size_t start = 0;
    // Where the current field begins.

    while (count < maxParts - 1)
    {
        size_t bar = line.find('|', start);
        // Find the next separator.

        if (bar == string::npos) break;
        // No more separators.

        parts[count++] = line.substr(start, bar - start);
        // Store the text before the separator.

        start = bar + 1;
        // Continue after the separator.
    }

    parts[count++] = line.substr(start);
    // The final field is the rest of the line.

    return count;
}

// ---------------------------------------------------------------------------
// LABORATORIES  (original Student A code, with robustness fixes)
// ---------------------------------------------------------------------------

// This function saves all laboratories into the resources file.
bool FileManager::saveToFile(LinkedList& list, string filename)
{
    ofstream file(filename.c_str());
    // Open (and overwrite) the file.

    if (!file.is_open())
    {
        cout << "ERROR: Could not open file for writing: " << filename << endl;
        cout << "Check that the 'data' folder exists and that you have permission to write to it." << endl;
        return false;
    }

    // This variable will store the number of laboratories.
    int count;

    // Create a temporary dynamic array containing all laboratories.
    // The toArray function will put the number of laboratories into count.
    Laboratory* labs = list.toArray(count);

    // Go through every laboratory.
    for (int i = 0; i < count; i++)
    {
        // Write all seven laboratory fields into the file.
        // The | symbol separates the different fields.
        file << labs[i].getId() << "|"
             << labs[i].getName() << "|"
             << labs[i].getType() << "|"
             << labs[i].getBuilding() << "|"
             << labs[i].getCapacity() << "|"
             << labs[i].getStatus() << "|"
             << labs[i].getCondition()
             << endl;
    }

    // Delete the temporary array because we created it with new.
    delete[] labs;

    // Close the file.
    file.close();

    // Tell the program that saving was successful.
    return true;
}

// This function loads laboratories from the resources file.
bool FileManager::loadFromFile(LinkedList& list, string filename)
{
    ifstream file(filename.c_str());

    if (!file.is_open())
    {
        // The file does not exist yet (first run) - the caller prints a friendly message.
        return false;
    }

    // Remove the laboratories currently in memory.
    list.clear();

    // This variable will store one complete line from the file.
    string line;

    // Read the file one line at a time.
    while (getline(file, line))
    {
        stripCarriageReturn(line);
        // Handle Windows line endings.

        if (line.empty()) continue;
        // Ignore blank lines.

        string parts[7];
        // The seven laboratory fields.

        if (splitFields(line, parts, 7) != 7)
        {
            // If the line does not contain all separators, ignore that line.
            continue;
        }

        // Convert the capacity from text into an integer.
        int capacity = atoi(parts[4].c_str());

        // Create a Laboratory object.
        Laboratory lab(
            parts[0],   // id
            parts[1],   // name
            parts[2],   // type
            parts[3],   // building
            capacity,
            parts[5],   // status
            parts[6]    // condition
        );

        // Add the laboratory to the linked list.
        list.addEnd(lab);
    }

    // Close the file.
    file.close();

    // Tell the program that loading was successful.
    return true;
}

// ---------------------------------------------------------------------------
// REQUESTS  (added during integration)
// ---------------------------------------------------------------------------

// Writes one request as a line of the requests file.
static void writeRequestLine(ofstream& file, const Request& r, char queueCode)
{
    file << r.requestId << "|" << r.studentId << "|" << r.laboratory << "|"
         << r.item << "|" << r.requestType << "|" << r.priority << "|"
         << r.status << "|" << r.requestTime << "|" << queueCode << endl;
}

bool FileManager::saveRequests(const RequestQueue& normal, const PriorityRequestQueue& priority,
                               const RequestQueue& history, string filename)
{
    ofstream file(filename.c_str());
    if (!file.is_open())
    {
        cout << "ERROR: Could not open file for writing: " << filename << endl;
        return false;
    }

    int count = 0;
    Request* array = NULL;
    // Each queue is copied into an array (front first), so saving does not disturb the queue.

    array = normal.toArray(count);
    for (int i = 0; i < count; i++) writeRequestLine(file, array[i], 'N');
    delete[] array;
    // Normal (FIFO) requests, in arrival order.

    array = priority.toArray(count);
    for (int i = 0; i < count; i++) writeRequestLine(file, array[i], 'P');
    delete[] array;
    // Priority requests, most urgent first.

    array = history.toArray(count);
    for (int i = 0; i < count; i++) writeRequestLine(file, array[i], 'H');
    delete[] array;
    // Already processed requests.

    return true;
}

bool FileManager::loadRequests(RequestQueue& normal, PriorityRequestQueue& priority,
                               RequestQueue& history, string filename, int& nextRequestId)
{
    ifstream file(filename.c_str());
    if (!file.is_open()) return false;

    string line;
    int largestId = 0;
    // Used to continue numbering after the last saved request.

    while (getline(file, line))
    {
        stripCarriageReturn(line);
        if (line.empty()) continue;

        string parts[9];
        if (splitFields(line, parts, 9) != 9) continue;
        // Skip damaged lines.

        Request r;
        r.requestId   = atoi(parts[0].c_str());
        r.studentId   = parts[1];
        r.laboratory  = parts[2];
        r.item        = parts[3];
        r.requestType = parts[4];
        r.priority    = atoi(parts[5].c_str());
        r.status      = parts[6];
        r.requestTime = parts[7];
        // Rebuild the request from the text fields.

        if (r.priority < 1 || r.priority > 4) r.priority = 3;
        // Repair an invalid priority.

        if (r.requestId > largestId) largestId = r.requestId;

        if (parts[8] == "P")      priority.insert(r, false);   // quiet insert (no message)
        else if (parts[8] == "H") history.enqueue(r, false);
        else                      normal.enqueue(r, false);
        // Put the request back in the right structure.
    }

    nextRequestId = largestId + 1;
    return true;
}

// ---------------------------------------------------------------------------
// LOCATIONS / GRAPH  (added during integration)
// ---------------------------------------------------------------------------

bool FileManager::saveLocations(const CampusGraph& graph, string filename)
{
    ofstream file(filename.c_str());
    if (!file.is_open())
    {
        cout << "ERROR: Could not open file for writing: " << filename << endl;
        return false;
    }
    graph.writeTo(file);
    // The graph knows how to write itself.
    return true;
}

bool FileManager::loadLocations(CampusGraph& graph, string filename)
{
    ifstream file(filename.c_str());
    if (!file.is_open()) return false;

    graph.clear();
    // Start from an empty graph.

    string line;
    string pendingEdges[200][2];
    int pendingCount = 0;
    // Edges are applied AFTER all locations are loaded, so the order of lines in the file does not matter.

    while (getline(file, line))
    {
        stripCarriageReturn(line);
        if (line.empty()) continue;

        string parts[4];
        int found = splitFields(line, parts, 4);

        if (found >= 2 && parts[0] == "LOCATION")
        {
            graph.addLocation(parts[1]);
        }
        else if (found >= 3 && parts[0] == "EDGE" && pendingCount < 200)
        {
            pendingEdges[pendingCount][0] = parts[1];
            pendingEdges[pendingCount][1] = parts[2];
            pendingCount++;
            // Remember the edge for later (a 4th field such as a distance is ignored).
        }
    }

    for (int i = 0; i < pendingCount; i++)
    {
        graph.addConnection(pendingEdges[i][0], pendingEdges[i][1]);
        // Now that every location exists, connect them.
    }

    return true;
}

// ---------------------------------------------------------------------------
// ACTIVITIES  (added during integration)
// ---------------------------------------------------------------------------

bool FileManager::saveActivities(const ActivityScheduler& scheduler, string filename)
{
    ofstream file(filename.c_str());
    if (!file.is_open())
    {
        cout << "ERROR: Could not open file for writing: " << filename << endl;
        return false;
    }

    int count = 0;
    Activity* array = scheduler.toArray(count);
    for (int i = 0; i < count; i++)
    {
        file << array[i].id << "|" << array[i].name << "|" << array[i].venue << "|"
             << array[i].date << "|" << array[i].time << "|" << array[i].priority << endl;
    }
    delete[] array;
    return true;
}

bool FileManager::loadActivities(ActivityScheduler& scheduler, string filename)
{
    ifstream file(filename.c_str());
    if (!file.is_open()) return false;

    scheduler.clear();

    string line;
    while (getline(file, line))
    {
        stripCarriageReturn(line);
        if (line.empty()) continue;

        string parts[6];
        if (splitFields(line, parts, 6) != 6) continue;

        Activity a;
        a.id = atoi(parts[0].c_str());
        a.name = parts[1];
        a.venue = parts[2];
        a.date = parts[3];
        a.time = parts[4];
        a.priority = atoi(parts[5].c_str());

        scheduler.addActivity(a, false);
        // Quiet insertion; clashes inside a damaged file are simply skipped.
    }
    return true;
}
