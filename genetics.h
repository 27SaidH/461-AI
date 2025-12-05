#pragma once
#include <vector>
#include "data.h"
#include "fitness.h"

struct GAResult {
    Schedule bestSchedule;
    double bestFitness;
};

GAResult runGA(
    const std::vector<Activity>& activities,
    const std::vector<Room>& rooms,
    const std::vector<std::string>& timeSlots,
    const std::vector<Facilitator>& facs
);
