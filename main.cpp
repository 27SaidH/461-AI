#include <iostream>
#include <fstream>
#include "data.h"
#include "fitness.h"
#include "genetic.h"
#include <map>

// ASCII bar chart helper (extra credit)
std::string bar(int count, int max = 20) {
    int len = std::min(count, max);
    return std::string(len, '#');
}

int main() {
    std::vector<Activity> activities;
    std::vector<Room> rooms;
    std::vector<std::string> timeSlots;
    std::vector<Facilitator> facs;

    loadData(activities, rooms, timeSlots, facs);


    GAResult result = runGA(activities, rooms, timeSlots, facs);

    // Evaluate violations for reporting
    FitnessResult stats = evaluateSchedule(result.bestSchedule, activities, rooms, timeSlots);

   
    std::ofstream out("best_schedule.txt");
    out << "Best Fitness = " << result.bestFitness << "\n\n";

    out << "Constraint Summary:\n";
    out << "Room Conflicts: " << stats.roomConflicts << "\n";
    out << "Facilitator Conflicts: " << stats.facilitatorConflicts << "\n";
    out << "Room Size Violations: " << stats.roomSizeViolations << "\n";
    out << "Special Violations: " << stats.specialViolations << "\n\n";

    out << "Activity,Room,Time,Facilitator\n";
    for (const auto& a : result.bestSchedule) {
        out << a.activityName << ","
            << a.roomName << ","
            << a.timeSlot << ","
            << a.facilitator << "\n";
    }
    out.close();


    // ASCII BAR CHARTS
  
    std::cout << "\n=== Constraint Violation Bar Chart ===\n";
    std::cout << "Room Conflicts        | " << bar(stats.roomConflicts) << " (" << stats.roomConflicts << ")\n";
    std::cout << "Facilitator Conflicts | " << bar(stats.facilitatorConflicts) << " (" << stats.facilitatorConflicts << ")\n";
    std::cout << "Room Size Violations  | " << bar(stats.roomSizeViolations) << " (" << stats.roomSizeViolations << ")\n";
    std::cout << "Special Violations    | " << bar(stats.specialViolations) << " (" << stats.specialViolations << ")\n";


    std::ofstream vcsv("violations_report.csv");
    vcsv << "Violation,Count\n";
    vcsv << "Room Conflicts," << stats.roomConflicts << "\n";
    vcsv << "Facilitator Conflicts," << stats.facilitatorConflicts << "\n";
    vcsv << "Room Size Violations," << stats.roomSizeViolations << "\n";
    vcsv << "Special Violations," << stats.specialViolations << "\n";
    vcsv.close();
    std::cout << "\nGenerated: violations_report.csv\n";

   // Room Utilization
  
    auto roomUtil = computeRoomUtilization(result.bestSchedule);

    std::ofstream roomCSV("room_utilization.csv");
    roomCSV << "Room,Count\n";
    for (auto& p : roomUtil) {
        roomCSV << p.first << "," << p.second << "\n";
    }
    roomCSV.close();
    std::cout << "Generated: room_utilization.csv\n";


    auto facLoad = computeFacilitatorLoad(result.bestSchedule);

    std::ofstream facCSV("facilitator_load.csv");
    facCSV << "Facilitator,Count\n";
    for (auto& p : facLoad) {
        facCSV << p.first << "," << p.second << "\n";
    }
    facCSV.close();
    std::cout << "Generated: facilitator_load.csv\n";

   
    // Final Output
   
    std::cout << "\n------------------------------------------\n";
    std::cout << " Genetic Algorithm\n";
    std::cout << "------------------------------------------\n";
    std::cout << "Best fitness: " << result.bestFitness << "\n";
    std::cout << "Best schedule saved to: best_schedule.txt\n";
    std::cout << "Fitness log saved to: fitness_over_time.csv\n";
    std::cout << " Additional CSVs saved:\n";
    std::cout << "  - violations_report.csv\n";
    std::cout << "  - room_utilization.csv\n";
    std::cout << "  - facilitator_load.csv\n";
    std::cout << "ASCII charts displayed n\n";

    return 0;
}
