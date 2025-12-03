#pragma once
#include "data.h"
#include "fitness.h"

struct GAResult {
    Schedule bestSchedule;
    double bestFitness;
};

GAResult runGA(int populationSize, int maxGenerations);
