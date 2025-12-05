#pragma once
#include "data.h"
#include <vector>
#include <string>
#include <map>

struct FitnessResult {
    double fitness = 0.0;
    int roomConflicts = 0;
    int facilitatorConflicts = 0;
    int roomSizeViolations = 0;
    int specialViolations = 0;
};

FitnessResult evaluateSchedule(
    const Schedule& sched,
    const std::vector<Activity>& activities,
    const std::vector<Room>& rooms,
    const std::vector<std::string>& timeSlots
);

// EXTRA CREDIT REPORTING HELPERS
std::map<std::string, int> computeRoomUtilization(const Schedule& sched);
std::map<std::string, int> computeFacilitatorLoad(const Schedule& sched);
