#pragma once
#include <string>
#include <vector>
#include <random>
using namespace std;

struct Activity {
    string name;
    int expectedEnrollment;
    vector<string> preferred;
    vector<string> others;
    bool needsLab;
    bool needsProjector;
};

struct Room {
    string name;
    int capacity;
    bool hasLab;
    bool hasProjector;
};

struct Assignment {
    string activityName;
    string roomName;
    string timeSlot;
    string facilitator;
};

using Schedule = vector<Assignment>;

struct FitnessResult {
    double fitness;
    int roomConflicts;
    int facilitatorOverload;
    int roomSizeViolations;
    int specialRuleViolations;
};


static vector<string> FACILITATORS = {
    "Lock","Glen","Banks","Richards","Shaw","Singer",
    "Uther","Tyler","Numen","Zeldin"
};

static vector<string> TIME_SLOTS = {
    "10 AM","11 AM","12 PM","1 PM","2 PM","3 PM"
};

// 11 activities
static vector<Activity> ACTIVITIES = {
    {"SLA101A",40,{"Glen","Lock","Banks"},{"Numen","Richards","Shaw","Singer"},false,false},
    {"SLA101B",35,{"Glen","Lock","Banks"},{"Numen","Richards","Shaw","Singer"},false,false},
    {"SLA191A",45,{"Glen","Lock","Banks"},{"Numen","Richards","Shaw","Singer"},true,false},
    {"SLA191B",40,{"Glen","Lock","Banks"},{"Numen","Richards","Shaw","Singer"},true,false},
    {"SLA201",60,{"Glen","Banks","Zeldin","Lock","Singer"},{"Richards","Uther","Shaw"},false,false},
    {"SLA291",50,{"Glen","Banks","Zeldin","Lock","Singer"},{"Richards","Uther","Shaw"},true,false},
    {"SLA303",25,{"Glen","Zeldin"},{"Banks"},true,true},
    {"SLA304",20,{"Singer","Uther"},{"Richards"},true,false},
    {"SLA394",15,{"Tyler","Singer"},{"Richards","Zeldin"},false,false},
    {"SLA449",30,{"Tyler","Zeldin","Uther"},{"Zeldin","Shaw"},false,true},
    {"SLA451",90,{"Lock","Banks","Zeldin"},{"Tyler","Singer","Shaw","Glen"},true,true}
};

static vector<Room> ROOMS = {
    {"Beach 201",18,false,true},
    {"Beach 301",25,true,true},
    {"Frank 119",95,true,true},
    {"Loft 206",55,false,false},
    {"Loft 310",48,true,false},
    {"James 325",110,true,true},
    {"Roman 201",40,false,false},
    {"Roman 216",80,true,true},
    {"Slater 003",32,true,true}
};

static mt19937& rng() {
    static random_device rd;
    static mt19937 gen(rd());
    return gen;
}

// Forward declaration
FitnessResult evaluateSchedule(const Schedule&);

inline Schedule randomSchedule() {
    Schedule s;
    uniform_int_distribution<int> roomDist(0, ROOMS.size() - 1);
    uniform_int_distribution<int> facDist(0, FACILITATORS.size() - 1);
    uniform_int_distribution<int> timeDist(0, TIME_SLOTS.size() - 1);

    for (auto& a : ACTIVITIES) {
        Assignment asg;
        asg.activityName = a.name;
        asg.roomName = ROOMS[roomDist(rng())].name;
        asg.timeSlot = TIME_SLOTS[timeDist(rng())];
        asg.facilitator = FACILITATORS[facDist(rng())];
        s.push_back(asg);
    }
    return s;
}
