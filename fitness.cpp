#include "fitness.h"
#include <unordered_map>

// =====================================================
// SIMPLE FITNESS FUNCTION (Minimum required rules)
// =====================================================
FitnessResult evaluateSchedule(const Schedule& sched) {
    FitnessResult result{0,0,0,0,0};

    unordered_map<string, vector<int>> roomTimes;
    unordered_map<string, vector<int>> facTimes;
    unordered_map<string, int> facLoad;

    // Build helper structures
    for (int i = 0; i < sched.size(); i++) {
        auto& a = sched[i];
        int tIndex = -1;
        for (int t = 0; t < TIME_SLOTS.size(); t++)
            if (TIME_SLOTS[t] == a.timeSlot) tIndex = t;

        roomTimes[a.roomName].push_back(tIndex);
        facTimes[a.facilitator].push_back(tIndex);
        facLoad[a.facilitator]++;
    }

    // Evaluate all assignments
    for (auto& asg : sched) {
        const Activity* act = nullptr;
        for (auto& a : ACTIVITIES)
            if (a.name == asg.activityName)
                act = &a;

        const Room* room = nullptr;
        for (auto& r : ROOMS)
            if (r.name == asg.roomName)
                room = &r;

        // ---------------- ROOM CONFLICTS ----------------
        auto& rt = roomTimes[asg.roomName];
        for (int i = 0; i < rt.size(); i++)
            for (int j = i + 1; j < rt.size(); j++)
                if (rt[i] == rt[j])
                    result.roomConflicts--, result.fitness -= 0.5;

        // ---------------- ROOM SIZE ----------------
        if (room->capacity < act->expectedEnrollment) {
            result.roomSizeViolations--;
            result.fitness -= 0.5;
        }
        else {
            result.fitness += 0.3;
        }

        // ---------------- FACILITATOR LOAD ----------------
        if (facLoad[asg.facilitator] > 4) {
            result.facilitatorOverload--;
            result.fitness -= 0.5;
        }
        else {
            result.fitness += 0.2;
        }

        // Preferred facilitator bonus
        bool preferred = false;
        for (auto& p : act->preferred)
            if (p == asg.facilitator)
                preferred = true;

        if (preferred) result.fitness += 0.5;
        else result.fitness -= 0.1;
    }

    return result;
}
