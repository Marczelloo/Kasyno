//
// Created by moskw on 09.10.2025.
//

#ifndef KASYNO_RNG_H
#define KASYNO_RNG_H
#include <random>


class Rng {
    std::mt19937_64 engine;
public:
    Rng();
    ~Rng();

    int randInt(int min, int max);
    double randDouble(double min, double max);
    bool randBool(double probability);
    std::mt19937_64& getEngine();
};


#endif //KASYNO_RNG_H