// Request.h   (Student B)
// Describes ONE request for a campus resource. It is shared by the normal queue,
// the priority queue and the processed-history log.
// INTEGRATION CHANGES:
//   * "laboratory" now holds the laboratory/resource ID (so it can be looked up in the Hash Table).
//   * "requestTime" was added because the brief requires a request time.
#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <ctime>
// <ctime> lets us read the computer clock for the request time.

// Returns the current date and time as text, e.g. "2026-11-02 10:15:30".
// The format sorts correctly as plain text (year first), which the sorting module relies on.
inline std::string currentTimestamp()
{
    time_t now = time(NULL);
    // Read the current clock time.

    char buffer[32];
    // A small character array that will hold the formatted text.

    struct tm* parts = localtime(&now);
    // Break the time into year, month, day, hour, minute, second.

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", parts);
    // Format the time as YYYY-MM-DD HH:MM:SS.

    return std::string(buffer);
    // Give the text back to the caller.
}

struct Request {
    int requestId;               // Unique request number (1, 2, 3 ...).
    std::string studentId;       // Who submitted the request (user information).
    std::string laboratory;      // ID of the requested resource (e.g. LAB101).
    std::string item;            // What exactly is needed (equipment / purpose).
    std::string requestType;     // booking / equipment / maintenance / access.
    int priority;                // 1 = highest priority, larger numbers = lower priority
    std::string status;          // Pending, Approved or Rejected ...
    std::string requestTime;     // When the request was submitted.

    // Default constructor: creates an empty request (used before a real one is read in).
    Request()
        : requestId(0), priority(3), status("Pending"), requestTime("") {}

    // Full constructor: creates a complete request. Status starts as "Pending" and the time is "now".
    Request(int id, const std::string& student,
            const std::string& lab, const std::string& requestedItem,
            const std::string& type, int requestPriority)
        : requestId(id), studentId(student), laboratory(lab),
          item(requestedItem), requestType(type),
          priority(requestPriority), status("Pending"),
          requestTime(currentTimestamp()) {}
};

#endif
