#include "genetic.h"
#include <random>
#include <algorithm>
#include <fstream>
#include <cmath>

static std::mt19937 rng(std::random_device{}());

// ---------------------------------------------------
// randomSchedule
// ---------------------------------------------------
Schedule randomSchedule(
    const std::vector<Activity>& acts,
    const std::vector<Room>& rooms,
    const std::vector<std::string>& times,
    const std::vector<Facilitator>& facs
) {
    Schedule s;
    std::uniform_int_distribution<int> rDist(0, rooms.size() - 1);
    std::uniform_int_distribution<int> tDist(0, times.size() - 1);
    std::uniform_int_distribution<int> fDist(0, facs.size() - 1);

    for (auto& a : acts) {
        s.push_back({
            a.name,
            rooms[rDist(rng)].name,
            times[tDist(rng)],
            facs[fDist(rng)].name
            });
    }
    return s;
}

// ---------------------------------------------------
// softmax
// ---------------------------------------------------
std::vector<double> softmax(const std::vector<double>& fitnesses) {
    double maxF = *std::max_element(fitnesses.begin(), fitnesses.end());
    std::vector<double> out(fitnesses.size());

    double sum = 0;
    for (int i = 0; i < fitnesses.size(); i++) {
        out[i] = std::exp(fitnesses[i] - maxF);
        sum += out[i];
    }
    for (double& v : out) v /= sum;
    return out;
}

// ---------------------------------------------------
// selectParent (roulette selection)
// ---------------------------------------------------
int selectParent(const std::vector<double>& probs) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double r = dist(rng);
    double acc = 0;

    for (int i = 0; i < probs.size(); i++) {
        acc += probs[i];
        if (r <= acc) return i;
    }
    return probs.size() - 1;
}

// ---------------------------------------------------
// crossover
// ---------------------------------------------------
Schedule crossover(const Schedule& p1, const Schedule& p2) {
    Schedule child = p1;
    std::uniform_int_distribution<int> cutDist(0, p1.size() - 1);
    int cut = cutDist(rng);

    for (int i = cut; i < p1.size(); i++) {
        child[i] = p2[i];
    }
    return child;
}

// ---------------------------------------------------
// mutate
// ---------------------------------------------------
void mutate(
    Schedule& s,
    const std::vector<Room>& rooms,
    const std::vector<std::string>& times,
    const std::vector<Facilitator>& facs,
    double rate
) {
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<int> pickField(0, 2);
    std::uniform_int_distribution<int> rDist(0, rooms.size() - 1);
    std::uniform_int_distribution<int> tDist(0, times.size() - 1);
    std::uniform_int_distribution<int> fDist(0, facs.size() - 1);

    for (auto& a : s) {
        if (prob(rng) < rate) {
            int field = pickField(rng);
            if (field == 0) a.roomName = rooms[rDist(rng)].name;
            else if (field == 1) a.timeSlot = times[tDist(rng)];
            else a.facilitator = facs[fDist(rng)].name;
        }
    }
}

// ---------------------------------------------------
// runGA — MAIN GENETIC ALGORITHM
// ---------------------------------------------------
GAResult runGA(
    const std::vector<Activity>& acts,
    const std::vector<Room>& rooms,
    const std::vector<std::string>& times,
    const std::vector<Facilitator>& facs
) {
    const int POP = 250;
    double mutationRate = 0.01;

    Population pop;
    pop.reserve(POP);

    // Initialize random population
    for (int i = 0; i < POP; i++)
        pop.push_back(randomSchedule(acts, rooms, times, facs));

    // Fitness log
    std::ofstream log("fitness_over_time.csv");
    log << "Generation,Best,Average,Worst\n";

    // EXTRA CREDIT: Mutation rate log
    std::ofstream mutateLog("mutation_history.csv");
    mutateLog << "Generation,MutationRate\n";

    double prevAvg = 0;
    GAResult result{};
    result.bestFitness = -1e18;

    int gen = 0;

    while (true) {

        // Log mutation rate for this generation
        mutateLog << gen << "," << mutationRate << "\n";

        std::vector<double> f(POP);
        double sum = 0;
        double best = -1e18;
        double worst = 1e18;
        int bestIdx = 0;

        // ----- FITNESS EVALUATION -----
        for (int i = 0; i < POP; i++) {
            auto fr = evaluateSchedule(pop[i], acts, rooms, times);
            f[i] = fr.fitness;
            sum += f[i];

            if (f[i] > best) { best = f[i]; bestIdx = i; }
            if (f[i] < worst) worst = f[i];
        }

        double avg = sum / POP;
        double improvement = (gen > 0 && prevAvg > 0)
            ? ((avg - prevAvg) / prevAvg) * 100.0
            : 0;

        // Log fitness stats
        log << gen << "," << best << "," << avg << "," << worst << "\n";

        // Track best overall
        if (best > result.bestFitness) {
            result.bestFitness = best;
            result.bestSchedule = pop[bestIdx];
        }

        // ----- STOPPING CRITERIA -----
        if (gen >= 100 && std::abs(improvement) < 1.0)
            break;

        prevAvg = avg;

        // ----- SELECTION -----
        auto probs = softmax(f);

        // ----- NEXT GENERATION -----
        Population nextPop;
        nextPop.reserve(POP);

        while (nextPop.size() < POP) {
            int p1 = selectParent(probs);
            int p2 = selectParent(probs);
            if (p1 == p2) continue;

            Schedule c1 = crossover(pop[p1], pop[p2]);
            Schedule c2 = crossover(pop[p2], pop[p1]);

            mutate(c1, rooms, times, facs, mutationRate);
            mutate(c2, rooms, times, facs, mutationRate);

            nextPop.push_back(c1);
            if (nextPop.size() < POP) nextPop.push_back(c2);
        }

        pop = nextPop;
        gen++;
    }

    return result;
}
