// This file contains the actual implementation of the laboratory class
#include "Laboratory.h"
// We include the header because this file implements Laboratory.

Laboratory::Laboratory()
{
    id = "";
    // The initial ID starts empty.

    name = "";
    // The initial name starts empty.

    type = "";
    // The initial type starts empty.

    building = "";
    // The initial building starts empty.

    capacity = 0;
    // The initial capacity is zero.

    status = "";
    // The initial status starts empty.

    condition = "";
    // The initial condition starts empty.
}

Laboratory::Laboratory(string id, string name, string type,
                       string building, int capacity,
                       string status, string condition)
{
    this->id = id;
    // Store the supplied ID inside the object.

    this->name = name;
    // Store the supplied name.

    this->type = type;
    // Store the supplied type.

    this->building = building;
    // Store the supplied building.

    this->capacity = capacity;
    // Store the supplied capacity.

    this->status = status;
    // Store the supplied status.

    this->condition = condition;
    // Store the supplied condition.
}
const string& Laboratory::getId() const
{
    return id;
    // Give the caller the laboratory ID.
}
const string& Laboratory::getName() const
{
    return name;
    // Give the caller the laboratory name.
}
const string& Laboratory::getType() const
{
    return type;
    // Give the caller the laboratory type.
}
const string& Laboratory::getBuilding() const
{
    return building;
    // Give the caller the building.
}
int Laboratory::getCapacity() const
{
    return capacity;
    // Give the caller the capacity.
}
const string& Laboratory::getStatus() const
{
    return status;
    // Give the caller the status.
}
const string& Laboratory::getCondition() const
{
    return condition;
    // Give the caller the condition.
}

void Laboratory::setId(string id)
{
    this->id = id;
    // Change the laboratory ID.
}

void Laboratory::setName(string name)
{
    this->name = name;
    // Change the laboratory name.
}

void Laboratory::setType(string type)
{
    this->type = type;
    // Change the laboratory type.
}

void Laboratory::setBuilding(string building)
{
    this->building = building;
    // Change the laboratory building.
}

void Laboratory::setCapacity(int capacity)
{
    this->capacity = capacity;
    // Change the laboratory capacity.
}

void Laboratory::setStatus(string status)
{
    this->status = status;
    // Change the laboratory status.
}

void Laboratory::setCondition(string condition)
{
    this->condition = condition;
    // Change the laboratory condition.
}
