// CampusGraph.cpp   (Student C)
// Implementation of the campus graph.
#include "CampusGraph.h"

// Constructor: no locations, every adjacency list empty.
CampusGraph::CampusGraph() {
    count = 0;

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        adjacency[i] = nullptr;
    }
}

// Destructor: free all edge nodes.
CampusGraph::~CampusGraph() {
    clear();
}

// Delete all edges and forget all locations.
void CampusGraph::clear() {
    for (int i = 0; i < count; i++) {
        Edge* current = adjacency[i];

        while (current != nullptr) {
            Edge* next = current->next;
            delete current;
            current = next;
        }
        adjacency[i] = nullptr;
        locations[i] = "";
    }
    count = 0;
}

// Linear search for a location name. O(V).
int CampusGraph::findLocation(const std::string& name) const {
    for (int i = 0; i < count; i++) {
        if (locations[i] == name) return i;
    }

    return -1;
    // Unknown location.
}

int CampusGraph::locationCount() const {
    return count;
}

// ADD LOCATION: reject empty names, duplicates and a full graph.
bool CampusGraph::addLocation(const std::string& name) {
    if (name.empty() ||
        count >= MAX_LOCATIONS ||
        findLocation(name) != -1) {
        return false;
    }

    locations[count] = name;
    adjacency[count] = nullptr;
    count++;
    return true;
}

// ADD CONNECTION: an undirected edge is stored in BOTH locations' lists.
bool CampusGraph::addConnection(
    const std::string& from,
    const std::string& to) {

    int a = findLocation(from);
    int b = findLocation(to);

    if (a == -1 || b == -1 || a == b) return false;
    // Both locations must exist and must be different.

    Edge* current = adjacency[a];

    while (current != nullptr) {
        if (current->destination == b) return false;
        // The connection already exists.

        current = current->next;
    }

    adjacency[a] = new Edge(b, adjacency[a]);
    // Add b to a's list (at the front).

    adjacency[b] = new Edge(a, adjacency[b]);
    // Add a to b's list (undirected: the path works both ways).

    return true;
}

// BREADTH-FIRST SEARCH: visit all neighbours first, then their neighbours ... Time O(V + E).
void CampusGraph::BFS(const std::string& start) const {
    int source = findLocation(start);

    if (source == -1) {
        std::cout << "Location not found\n";
        return;
    }

    bool visited[MAX_LOCATIONS] = {};
    // Marks locations already reached (so we never visit twice).

    int queue[MAX_LOCATIONS];
    int front = 0;
    int rear = 0;
    // A simple array-based FIFO queue of vertex numbers.

    visited[source] = true;
    queue[rear++] = source;
    // Start by enqueuing the source.

    while (front < rear) {
        int vertex = queue[front++];
        // Dequeue the next location.

        std::cout << locations[vertex] << " ";
        // "Visit" it.

        Edge* current = adjacency[vertex];

        while (current != nullptr) {
            int next = current->destination;

            if (!visited[next]) {
                visited[next] = true;
                queue[rear++] = next;
                // Enqueue every unvisited neighbour.
            }

            current = current->next;
        }
    }

    std::cout << '\n';
}

// Recursive part of DFS.
void CampusGraph::dfsVisit(
    int vertex, bool visited[]) const {

    visited[vertex] = true;
    std::cout << locations[vertex] << " ";
    // Visit this location.

    Edge* current = adjacency[vertex];

    while (current != nullptr) {
        int next = current->destination;

        if (!visited[next]) {
            dfsVisit(next, visited);
            // Go deeper before looking at the other neighbours.
        }

        current = current->next;
    }
}

// DEPTH-FIRST SEARCH: Time O(V + E).
void CampusGraph::DFS(const std::string& start) const {
    int source = findLocation(start);

    if (source == -1) {
        std::cout << "Location not found\n";
        return;
    }

    bool visited[MAX_LOCATIONS] = {};
    dfsVisit(source, visited);
    std::cout << '\n';
}

// SHORTEST PATH (fewest connections) using BFS: the first time BFS reaches a vertex is by the fewest edges.
int CampusGraph::shortestPath(
    const std::string& from,
    const std::string& to) const {

    int source = findLocation(from);
    int target = findLocation(to);

    if (source == -1 || target == -1) return -1;
    // Unknown location.

    bool visited[MAX_LOCATIONS] = {};
    int distance[MAX_LOCATIONS] = {};
    // distance[v] = number of connections from the source to v.

    int queue[MAX_LOCATIONS];

    int front = 0;
    int rear = 0;

    visited[source] = true;
    queue[rear++] = source;

    while (front < rear) {
        int vertex = queue[front++];

        if (vertex == target) {
            return distance[vertex];
            // Reached the destination.
        }

        Edge* current = adjacency[vertex];

        while (current != nullptr) {
            int next = current->destination;

            if (!visited[next]) {
                visited[next] = true;
                distance[next] = distance[vertex] + 1;
                queue[rear++] = next;
            }

            current = current->next;
        }
    }

    return -1;
    // Queue emptied without reaching the target: not connected.
}

// Print the actual route: BFS remembers each vertex's "parent", then we walk back from the target.
bool CampusGraph::printShortestRoute(
    const std::string& from,
    const std::string& to
) const {
    int source = findLocation(from);
    int target = findLocation(to);

    if (source == -1 || target == -1) {
        std::cout << "No route found.\n";
        return false;
    }

    bool visited[MAX_LOCATIONS] = {};
    int parent[MAX_LOCATIONS];
    int queue[MAX_LOCATIONS];

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        parent[i] = -1;
        // -1 means "no parent yet".
    }

    int front = 0;
    int rear = 0;

    visited[source] = true;
    queue[rear++] = source;

    while (front < rear && !visited[target]) {
        int current = queue[front++];

        Edge* edge = adjacency[current];

        while (edge != nullptr) {
            int next = edge->destination;

            if (!visited[next]) {
                visited[next] = true;
                parent[next] = current;
                // Remember how we got here.

                queue[rear++] = next;
            }

            edge = edge->next;
        }
    }

    if (!visited[target]) {
        std::cout << "No route found.\n";
        return false;
    }

    int path[MAX_LOCATIONS];
    int length = 0;

    for (int at = target; at != -1; at = parent[at]) {
        path[length++] = at;
        // Walk backwards from the target to the source.
    }

    std::cout << "Shortest route: ";

    for (int i = length - 1; i >= 0; i--) {
        std::cout << locations[path[i]];
        // Print in forward order.

        if (i > 0) {
            std::cout << " -> ";
        }
    }

    std::cout << '\n';

    return true;
}

// Two locations are connected if a path exists between them.
bool CampusGraph::isConnected(
    const std::string& from,
    const std::string& to) const {

    return shortestPath(from, to) != -1;
}

// Print every location with its neighbours.
void CampusGraph::display() const {
    if (count == 0) {
        std::cout << "The campus graph is empty.\n";
        return;
    }

    for (int i = 0; i < count; i++) {
        std::cout << locations[i] << ": ";

        Edge* current = adjacency[i];

        if (current == nullptr) {
            std::cout << "(no connections)";
        }

        while (current != nullptr) {
            std::cout << locations[current->destination];

            if (current->next != nullptr) {
                std::cout << " -> ";
            }

            current = current->next;
        }

        std::cout << '\n';
    }
}

// Silent DFS used only for counting components.
void CampusGraph::markReachable(int vertex, bool visited[]) const {
    visited[vertex] = true;

    Edge* current = adjacency[vertex];
    while (current != nullptr) {
        if (!visited[current->destination]) {
            markReachable(current->destination, visited);
        }
        current = current->next;
    }
}

// CONNECTED COMPONENTS: start a DFS from every unvisited location; each start is one new component. O(V + E).
int CampusGraph::connectedComponents() const {
    bool visited[MAX_LOCATIONS] = {};
    int components = 0;

    for (int i = 0; i < count; i++) {
        if (!visited[i]) {
            components++;
            // A new island was found.

            markReachable(i, visited);
            // Mark everything reachable from it.
        }
    }

    return components;
}

// Write the graph in the same format the loader reads (each edge only once: lower index first).
void CampusGraph::writeTo(std::ostream& out) const {
    for (int i = 0; i < count; i++) {
        out << "LOCATION|" << locations[i] << '\n';
    }

    for (int i = 0; i < count; i++) {
        Edge* current = adjacency[i];
        while (current != nullptr) {
            if (i < current->destination) {
                // Only write an edge from its smaller vertex, so it is not saved twice.

                out << "EDGE|" << locations[i] << '|'
                    << locations[current->destination] << '\n';
            }
            current = current->next;
        }
    }
}
