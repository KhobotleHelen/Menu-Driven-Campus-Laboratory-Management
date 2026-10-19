// CampusGraph.h   (Student C)
// Campus map as an UNDIRECTED, UNWEIGHTED graph stored as ADJACENCY LISTS.
//   Vertex = a campus location (Library, Lab A ...).     Edge = a pathway between two locations.
// Adjacency lists use O(V + E) memory (an adjacency matrix would use O(V^2)), which is better for a
// campus map where most locations connect to only a few others.
// Provided: BFS, DFS, connectivity check, shortest path (fewest connections), route printing,
//           and (INTEGRATION ADDITION) counting connected components + writing the graph to a file.
#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H

#include <string>
#include <iostream>

class CampusGraph {
private:
    static const int MAX_LOCATIONS = 30;
    // Maximum number of locations (fixed-size arrays keep the code simple).

    // One element of an adjacency list.
    struct Edge {
        int destination;    // Index of the neighbouring location.
        Edge* next;         // Next neighbour in the list.

        Edge(int value, Edge* link) {
            destination = value;
            next = link;
        }
    };

    std::string locations[MAX_LOCATIONS];   // Names of the locations (index = vertex number).
    Edge* adjacency[MAX_LOCATIONS];         // Head of each location's neighbour list.
    int count;                              // Number of locations currently in the graph.

    int findLocation(const std::string& name) const;
    // Converts a location name to its vertex number (-1 if unknown).

    void dfsVisit(int vertex, bool visited[]) const;
    // Recursive helper for DFS (prints as it visits).

    void markReachable(int vertex, bool visited[]) const;
    // Recursive helper that marks reachable vertices WITHOUT printing (used for components).

public:
    CampusGraph();
    ~CampusGraph();

    CampusGraph(const CampusGraph&) = delete;
    CampusGraph& operator=(const CampusGraph&) = delete;
    // Copying is forbidden (would share edge lists).

    bool addLocation(const std::string& name);
    bool addConnection(const std::string& from, const std::string& to);

    bool isConnected(const std::string& from, const std::string& to) const;
    int shortestPath(const std::string& from, const std::string& to) const;
    bool printShortestRoute(const std::string& from, const std::string& to) const;

    void BFS(const std::string& start) const;   // Breadth-First Search: level by level, uses a QUEUE.
    void DFS(const std::string& start) const;   // Depth-First Search: as deep as possible, uses RECURSION (the call STACK).
    void display() const;                       // Print every location and its neighbours.

    int connectedComponents() const;            // How many separate "islands" the campus map has.
    int locationCount() const;                  // Number of locations.
    void writeTo(std::ostream& out) const;      // Save format: LOCATION|name   and   EDGE|a|b
    void clear();                               // Remove everything (used when reloading data).
};

#endif
