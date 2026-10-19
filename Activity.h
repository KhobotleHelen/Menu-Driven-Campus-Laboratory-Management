// Activity.h   (Student B - Activity Scheduling)
// A scheduled laboratory activity (practical session, workshop, exam ...).
// INTEGRATION ADDITION: the original prototype only DISPLAYED a "scheduled" message.
// ActivityScheduler now STORES activities in a linked list kept ordered by date and time,
// REJECTS clashes (same venue, same date, same time) and can be saved to / loaded from a file.
#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <string>
#include <iostream>

struct Activity {
    int id;                 // Unique activity number.
    std::string name;       // What is happening (e.g. "Java practical").
    std::string venue;      // Laboratory / resource ID where it happens.
    std::string date;       // YYYY-MM-DD
    std::string time;       // HH:MM (24 hour)
    int priority;           // 1 = Emergency ... 4 = Low (same scale as requests)

    Activity() : id(0), priority(3) {}
    // Default constructor.
};

class ActivityScheduler {
private:
    // One link of the schedule chain.
    struct Node {
        Activity data;
        Node* next;
        explicit Node(const Activity& a) : data(a), next(nullptr) {}
    };

    Node* head;       // The EARLIEST activity.
    int count;        // Number of scheduled activities.
    int nextId;       // The ID the next new activity will receive.

public:
    ActivityScheduler() : head(nullptr), count(0), nextId(1) {}
    ~ActivityScheduler();

    ActivityScheduler(const ActivityScheduler&) = delete;
    ActivityScheduler& operator=(const ActivityScheduler&) = delete;

    // Returns 0 = scheduled, 1 = clash with an existing activity.  Assigns an ID when a.id == 0.
    int addActivity(Activity a, bool verbose = true);   // ordered insert, O(n)
    void display() const;                               // print the timetable, earliest first
    Activity* toArray(int& number) const;               // copy to new[] array (caller must delete[])
    int size() const;
    void clear();

    static bool validDate(const std::string& d);        // checks the shape YYYY-MM-DD
    static bool validTime(const std::string& t);        // checks the shape HH:MM
};

#endif
