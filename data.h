#pragma once
#include <string>
#include <vector>

struct Activity {
    std::string name;
    int expectedEnrollment;
    std::vector<std::string> preferred;
    std::vector<std::string> others;
    bool needsLab = false;
    bool needsProjector = false;
};

struct Room {
    std::string name;
    int capacity;
    bool hasLab = false;
    bool hasProjector = false;
};

struct Facilitator {
    std::string name;
};

struct Assignment {
    std::string activityName;
    std::string roomName;
    std::string timeSlot;
    std::string facilitator;
};

using Schedule = std::vector<Assignment>;
using Population = std::vector<Schedule>;

// Declaration — implemented in data.cpp
void loadData(
    std::vector<Activity>& acts,
    std::vector<Room>& rooms,
    std::vector<std::string>& timeSlots,
    std::vector<Facilitator>& facs
);
