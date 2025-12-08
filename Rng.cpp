//
// Created by moskw on 09.10.2025.
//

#include "Rng.h"

Rng::Rng() {
    std::random_device rd;
    engine.seed(rd());
}

Rng::~Rng() {}

int Rng::randInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

double Rng::randDouble(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(engine);
}

bool Rng::randBool(double probability) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(engine) < probability;
}

std::mt19937_64 &Rng::getEngine() {
    return engine;
}




