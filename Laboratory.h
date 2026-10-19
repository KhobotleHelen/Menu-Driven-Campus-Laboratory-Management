// Laboratory.h   (Student A - shared by ALL modules)
// INTEGRATION NOTE: getters are now "const" and return const references so the
// BST, Hash Table and other modules can read a Laboratory without copying it.
// This file describes what a Laboratory object contains
// and what operations can be performed on it.
#ifndef LABORATORY_H
#define LABORATORY_H
#include <string>
// We need string because several laboratory attributes are Strings
using namespace std;


class Laboratory
{
private:

    string id;
    string name;
    string type;
    string building;
    int capacity;
    string status;
    string condition;
public:

    Laboratory();
    // Default constructor.
    // Creates a laboratory with empty/default values.

    Laboratory(string id, string name, string type,
               string building, int capacity,
               string status, string condition);
    // Constructor that allows us to create a complete laboratory
    // by providing all seven attributes.

    const string& getId() const;
    // Returns the laboratory ID.

    const string& getName() const;
    // Returns the laboratory name.

    const string& getType() const;
    // Returns the laboratory type.

    const string& getBuilding() const;
    // Returns the laboratory building.

    int getCapacity() const;
    // Returns the laboratory capacity.

    const string& getStatus() const;
    // Returns the laboratory status.

    const string& getCondition() const;
    // Returns the laboratory condition.

    void setId(string id);
    // Changes the laboratory ID.

    void setName(string name);
    // Changes the laboratory name.

    void setType(string type);
    // Changes the laboratory type.

    void setBuilding(string building);
    // Changes the laboratory building.

    void setCapacity(int capacity);
    // Changes the laboratory capacity.

    void setStatus(string status);
    // Changes the laboratory status.

    void setCondition(string condition);
    // Changes the laboratory condition.
};

#endif
// Ends the Laboratory header file.
