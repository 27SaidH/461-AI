#include <iostream>
#include <fstream>
#include "ga.h"

int main() {
    cout << "Running Genetic Algorithm...\n";

    GAResult res = runGA(250, 150);

    cout << "\nBEST FITNESS = " << res.bestFitness << "\n";
    cout << "Writing schedule to best_schedule.txt...\n";

    ofstream out("best_schedule.txt");
    for (auto& a : res.bestSchedule) {
        out << a.activityName << " | "
            << a.timeSlot << " | "
            << a.roomName << " | "
            << a.facilitator << "\n";
    }

    cout << "Done.\n";
    return 0;
}
