#include "fitness.h"
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <map>

using std::string;
using std::vector;

static bool startsWith(const string& s, const string& prefix) {
    return s.rfind(prefix, 0) == 0;
}

FitnessResult evaluateSchedule(
    const Schedule& sched,
    const vector<Activity>& activities,
    const vector<Room>& rooms,
    const vector<std::string>& timeSlots
) {
    FitnessResult fr;
    double total = 0.0;

    // Lookup tables
    std::unordered_map<string, Activity> actMap;
    std::unordered_map<string, Room> roomMap;

    for (auto& a : activities) actMap[a.name] = a;
    for (auto& r : rooms) roomMap[r.name] = r;

    std::unordered_map<string, int> timeIndex;
    for (int i = 0; i < timeSlots.size(); i++)
        timeIndex[timeSlots[i]] = i;

    // Conflict tracking
    std::unordered_map<string, int> roomTimeCount;
    std::unordered_map<string, int> facTimeCount;
    std::unordered_map<string, int> facTotalCount;

    std::unordered_map<string, string> actTime;
    std::unordered_map<string, string> actRoom;

    // ------------------------------
    // First Pass — Count Usage
    // ------------------------------
    for (const auto& asg : sched) {
        string rt = asg.roomName + "|" + asg.timeSlot;
        string ft = asg.facilitator + "|" + asg.timeSlot;

        roomTimeCount[rt]++;
        facTimeCount[ft]++;
        facTotalCount[asg.facilitator]++;

        actTime[asg.activityName] = asg.timeSlot;
        actRoom[asg.activityName] = asg.roomName;
    }

    // ------------------------------
    // Second Pass — Per-Activity Fitness
    // ------------------------------
    for (const auto& asg : sched) {
        const Activity& act = actMap.at(asg.activityName);
        const Room& room = roomMap.at(asg.roomName);

        double f = 0.0;

        // ROOM SIZE
        double cap = room.capacity;
        double need = act.expectedEnrollment;
        if (cap < need) {
            f -= 0.5;
            fr.roomSizeViolations++;
        }
        else if (cap > 3 * need) {
            f -= 0.4;
            fr.roomSizeViolations++;
        }
        else if (cap > 1.5 * need) {
            f -= 0.2;
            fr.roomSizeViolations++;
        }
        else {
            f += 0.3;
        }

        // FACILITATOR MATCH QUALITY
        bool pref = std::find(act.preferred.begin(), act.preferred.end(), asg.facilitator) != act.preferred.end();
        bool other = std::find(act.others.begin(), act.others.end(), asg.facilitator) != act.others.end();

        if (pref) f += 0.5;
        else if (other) f += 0.2;
        else {
            f -= 0.1;
            fr.specialViolations++;
        }

        // EQUIPMENT
        if (act.needsLab || act.needsProjector) {
            int met = 0;
            if (act.needsLab && room.hasLab) met++;
            if (act.needsProjector && room.hasProjector) met++;

            if (met == 2) f += 0.2;
            else if (met == 1) {
                f -= 0.1;
                fr.specialViolations++;
            }
            else {
                f -= 0.3;
                fr.specialViolations++;
            }
        }

        // FACILITATOR LOAD AT TIMESLOT
        string ft = asg.facilitator + "|" + asg.timeSlot;
        int countAtTime = facTimeCount[ft];
        if (countAtTime == 1) f += 0.2;
        else if (countAtTime > 1) f -= 0.2;

        total += f;
    }

    // ------------------------------
    // SCHEDULE-LEVEL PENALTIES
    // ------------------------------

    // ROOM CONFLICTS
    for (auto& p : roomTimeCount) {
        if (p.second > 1) {
            int extra = p.second - 1;
            fr.roomConflicts += extra;
            total -= 0.5 * p.second;
        }
    }

    // FACILITATOR LOAD
    for (auto& p : facTotalCount) {
        string fac = p.first;
        int cnt = p.second;

        if (cnt > 4) {
            total -= 0.5 * cnt;
            fr.facilitatorConflicts += (cnt - 4);
        }
        else if (cnt < 3) {
            if (!(fac == "Tyler" && cnt < 2)) {
                total -= 0.4 * cnt;
                fr.facilitatorConflicts++;
            }
        }
    }

    // ------------------------------
    // SPECIAL SLA101 / SLA191 RULES
    // ------------------------------
    auto tIdx = [&](string name) {
        if (!actTime.count(name)) return -1;
        return timeIndex[actTime[name]];
        };

    auto isRB = [&](string room) {
        return startsWith(room, "Roman") || startsWith(room, "Beach");
        };

    int t101A = tIdx("SLA101A");
    int t101B = tIdx("SLA101B");
    if (t101A != -1 && t101B != -1) {
        int diff = abs(t101A - t101B);
        if (diff == 0) { total -= 0.5; fr.specialViolations++; }
        else if (diff >= 4) total += 0.5;
    }

    int t191A = tIdx("SLA191A");
    int t191B = tIdx("SLA191B");
    if (t191A != -1 && t191B != -1) {
        int diff = abs(t191A - t191B);
        if (diff == 0) { total -= 0.5; fr.specialViolations++; }
        else if (diff >= 4) total += 0.5;
    }

    vector<string> a101 = { "SLA101A","SLA101B" };
    vector<string> a191 = { "SLA191A","SLA191B" };

    for (auto& n191 : a191) {
        int t1 = tIdx(n191);
        if (t1 == -1) continue;
        string r1 = actRoom[n191];

        for (auto& n101 : a101) {
            int t2 = tIdx(n101);
            if (t2 == -1) continue;
            string r2 = actRoom[n101];

            int diff = abs(t1 - t2);

            if (diff == 0) {
                total -= 0.25;
                fr.specialViolations++;
            }
            else if (diff == 1) {
                total += 0.5;
                if (isRB(r1) != isRB(r2)) {
                    total -= 0.4;
                    fr.specialViolations++;
                }
            }
            else if (diff == 2) {
                total += 0.25;
            }
        }
    }

    fr.fitness = total;
    return fr;
}

// --------------------------------------------------------------
// EXTRA CREDIT: Room Utilization
// --------------------------------------------------------------
std::map<std::string, int> computeRoomUtilization(const Schedule& sched) {
    std::map<std::string, int> util;
    for (const auto& a : sched)
        util[a.roomName]++;
    return util;
}

// --------------------------------------------------------------
// EXTRA CREDIT: Facilitator Load
// --------------------------------------------------------------
std::map<std::string, int> computeFacilitatorLoad(const Schedule& sched) {
    std::map<std::string, int> load;
    for (const auto& a : sched)
        load[a.facilitator]++;
    return load;
}
