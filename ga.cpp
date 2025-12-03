#include "ga.h"
#include <algorithm>
#include <iostream>

using namespace std;

// =====================================================
// MUTATE A SINGLE SCHEDULE
// =====================================================
void mutate(Schedule& s, double rate) {
    uniform_real_distribution<double> prob(0, 1);

    for (auto& a : s) {
        if (prob(rng()) < rate) a.roomName = ROOMS[rand() % ROOMS.size()].name;
        if (prob(rng()) < rate) a.timeSlot = TIME_SLOTS[rand() % TIME_SLOTS.size()];
        if (prob(rng()) < rate) a.facilitator = FACILITATORS[rand() % FACILITATORS.size()];
    }
}

// =====================================================
// CROSSOVER TWO SCHEDULES
// =====================================================
Schedule crossover(const Schedule& p1, const Schedule& p2) {
    Schedule child = p1;
    for (int i = 0; i < child.size(); i++)
        if (rand() % 2)
            child[i] = p2[i];
    return child;
}

// =====================================================
// GENETIC ALGORITHM
// =====================================================
GAResult runGA(int populationSize, int maxGenerations) {
    vector<Schedule> pop(populationSize);

    // Initial population
    for (auto& s : pop)
        s = randomSchedule();

    double mutationRate = 0.02;
    Schedule bestSched;
    double bestFit = -9999;

    for (int gen = 0; gen < maxGenerations; gen++) {

        vector<pair<double, Schedule>> scored;

        // Score population
        for (auto& s : pop) {
            FitnessResult fr = evaluateSchedule(s);
            scored.push_back({fr.fitness, s});

            if (fr.fitness > bestFit) {
                bestFit = fr.fitness;
                bestSched = s;
            }
        }

        // Sort best → worst
        sort(scored.begin(), scored.end(),
            [](auto& a, auto& b) { return a.first > b.first; });

        // Elitism
        vector<Schedule> newPop;
        for (int i = 0; i < populationSize / 2; i++)
            newPop.push_back(scored[i].second);

        // Breed new children
        while (newPop.size() < populationSize) {
            int a = rand() % (populationSize / 2);
            int b = rand() % (populationSize / 2);
            Schedule child = crossover(scored[a].second, scored[b].second);
            mutate(child, mutationRate);
            newPop.push_back(child);
        }

        pop = newPop;

        // Optional dynamic mutation rate
        mutationRate *= 0.99;

        cout << "Gen " << gen << " | Best Fitness: " << bestFit << endl;
    }

    return {bestSched, bestFit};
}
