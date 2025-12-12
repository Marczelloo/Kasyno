//
// Created by moskw on 09.10.2025.
//

#include "Rng.h"
#include <stdexcept>
#include <chrono>
#include <cmath>
#include <windows.h>

#ifdef _WIN32
    #include <process.h>
#else
    #include <unistd.h>
#endif

uint64_t Rng::generateSeed() {
    uint64_t seed = 0;

    try {
        std::random_device rd;
        seed ^= static_cast<uint64_t>(rd()) << 32;
        seed ^= static_cast<uint64_t>(rd());
    } catch (const std::exception&) {
        seed = 0;
    }

    auto now = std::chrono::high_resolution_clock::now();
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(
        now.time_since_epoch()
    ).count();
    seed ^= static_cast<uint64_t>(nanos);

#ifdef _WIN32
    seed ^= static_cast<uint64_t>(_getpid()) << 16;
    seed ^= static_cast<uint64_t>(GetCurrentThreadId());
#else
    seed ^= static_cast<uint64_t>(getpid()) << 16;
    seed ^= static_cast<uint64_t>(pthread_self());
#endif

    if (seed == 0) {
        seed = std::chrono::steady_clock::now().time_since_epoch().count();
    }

    return seed;
}

Rng::Rng() {
    reseed();
}

void Rng::reseed() {
    engine.seed(generateSeed());
}

void Rng::reseed(uint64_t seed) {
    engine.seed(seed);
}

int Rng::randInt(int min, int max) {
    if (min > max) {
        throw std::invalid_argument(
            "Rng::randInt: min (" + std::to_string(min) +
            ") cannot be greater than max (" + std::to_string(max) + ")"
        );
    }

    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

double Rng::randDouble(double min, double max) {
    if (min > max) {
        throw std::invalid_argument(
            "Rng::randDouble: min (" + std::to_string(min) +
            ") cannot be greater than max (" + std::to_string(max) + ")"
        );
    }

    if (std::isnan(min) || std::isnan(max)) {
        throw std::invalid_argument("Rng::randDouble: min and max cannot be NaN");
    }

    if (std::isinf(min) || std::isinf(max)) {
        throw std::invalid_argument("Rng::randDouble: min and max cannot be infinity");
    }

    std::uniform_real_distribution<double> dist(min, max);
    return dist(engine);
}

bool Rng::randBool(double probability) {
    if (probability < 0.0 || probability > 1.0) {
        throw std::invalid_argument(
            "Rng::randBool: probability (" + std::to_string(probability) +
            ") must be in range [0.0, 1.0]"
        );
    }

    if (std::isnan(probability)) {
        throw std::invalid_argument("Rng::randBool: probability cannot be NaN");
    }

    if (probability <= 0.0) return false;
    if (probability >= 1.0) return true;

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(engine) < probability;
}




