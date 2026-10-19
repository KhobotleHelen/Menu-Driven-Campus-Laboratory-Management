// Activity.cpp   (Student B -)
#include "Activity.h"

// Destructor: free every node.
ActivityScheduler::~ActivityScheduler() {
    clear();
}

// Delete all activities.
void ActivityScheduler::clear() {
    while (head != nullptr) {
        Node* temporary = head;
        head = head->next;
        delete temporary;
    }
    count = 0;
    nextId = 1;
}

int ActivityScheduler::size() const {
    return count;
}

// Helper: is the character a digit?
static bool isDigitChar(char c) {
    return c >= '0' && c <= '9';
}

// A valid date has exactly 10 characters: 4 digits, '-', 2 digits, '-', 2 digits.
bool ActivityScheduler::validDate(const std::string& d) {
    if (d.size() != 10) return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            if (d[i] != '-') return false;
        } else if (!isDigitChar(d[i])) {
            return false;
        }
    }
    int month = (d[5] - '0') * 10 + (d[6] - '0');
    int day = (d[8] - '0') * 10 + (d[9] - '0');
    // Convert the digits to numbers so we can range-check them.

    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

// A valid time has exactly 5 characters: 2 digits, ':', 2 digits, hour 0-23 and minute 0-59.
bool ActivityScheduler::validTime(const std::string& t) {
    if (t.size() != 5 || t[2] != ':') return false;
    if (!isDigitChar(t[0]) || !isDigitChar(t[1]) || !isDigitChar(t[3]) || !isDigitChar(t[4])) return false;
    int hour = (t[0] - '0') * 10 + (t[1] - '0');
    int minute = (t[3] - '0') * 10 + (t[4] - '0');
    return hour <= 23 && minute <= 59;
}

// ADD: reject clashes, then insert so the list stays ordered by (date, time).
int ActivityScheduler::addActivity(Activity a, bool verbose) {
    // 1. Clash detection: same venue + date + time is not allowed. O(n).
    for (Node* c = head; c != nullptr; c = c->next) {
        if (c->data.venue == a.venue && c->data.date == a.date && c->data.time == a.time) {
            if (verbose) {
                std::cout << "Clash! " << a.venue << " is already used on " << a.date
                          << " at " << a.time << " by \"" << c->data.name << "\".\n";
            }
            return 1;
        }
    }

    // 2. Give the activity an ID if it does not have one yet.
    if (a.id == 0) {
        a.id = nextId;
    }
    if (a.id >= nextId) {
        nextId = a.id + 1;
        // Keep nextId larger than every ID in use (important after loading from a file).
    }

    Node* newNode = new Node(a);
    // 3. Ordered insertion (earlier date/time first; equal times: more urgent priority first).
    if (head == nullptr ||
        a.date < head->data.date ||
        (a.date == head->data.date && a.time < head->data.time)) {
        newNode->next = head;
        head = newNode;
        // New earliest activity.
    } else {
        Node* current = head;
        while (current->next != nullptr &&
               (current->next->data.date < a.date ||
                (current->next->data.date == a.date && current->next->data.time <= a.time))) {
            current = current->next;
            // Move past every activity that happens before (or at the same time as) the new one.
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    count++;

    if (verbose) {
        std::cout << "Activity #" << a.id << " scheduled: " << a.name << " | Venue: " << a.venue
                  << " | Date: " << a.date << " | Time: " << a.time << '\n';
    }
    return 0;
}

// Print the timetable, earliest first.
void ActivityScheduler::display() const {
    if (head == nullptr) {
        std::cout << "No activities are scheduled.\n";
        return;
    }
    std::cout << "\n--- Activity Timetable (earliest first) ---\n";
    for (Node* c = head; c != nullptr; c = c->next) {
        std::cout << "#" << c->data.id << " | " << c->data.date << " " << c->data.time
                  << " | Venue: " << c->data.venue << " | " << c->data.name
                  << " | Priority: " << c->data.priority << '\n';
    }
}

// Copy to a new array (used for saving).
Activity* ActivityScheduler::toArray(int& number) const {
    number = count;
    if (number == 0) return nullptr;
    Activity* array = new Activity[number];
    int i = 0;
    for (Node* c = head; c != nullptr; c = c->next) {
        array[i++] = c->data;
    }
    return array;
}
