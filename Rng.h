/**
 * @file Rng.h
 * @brief Random number generator wrapper class
 * @author Marczelloo
 * @date 2025-12-11
 */

//
// Created by moskw on 09.10.2025.
//

#ifndef KASYNO_RNG_H
#define KASYNO_RNG_H

#include <random>


/**
 * @class Rng
 * @brief Pseudo-random number generator using Mersenne Twister
 *
 * Provides convenient methods for generating:
 * - Random integers in a range
 * - Random doubles in a range
 * - Random booleans with probability
 */
class Rng {
    std::mt19937_64 engine;  ///< Mersenne Twister 64-bit engine

    /**
     * @brief Generates a high-quality seed from multiple sources
     * @return uint64_t Seed value
     */
    static uint64_t generateSeed();
public:
    /**
     * @brief Constructor - initializes RNG with random seed
     */
    Rng();

    /**
     * @brief Destructor
     */
    ~Rng() = default;

    Rng(const Rng&) = delete;
    Rng& operator=(const Rng&) = delete;

    Rng(Rng&&) noexcept = default;
    Rng& operator=(Rng&&) noexcept = default;

    /**
     * @brief Generates a random integer in the range [min, max]
     * @param min Minimum value (inclusive)
     * @param max Maximum value (inclusive)
     * @return int Random integer in range
     */
    int randInt(int min, int max);

    /**
     * @brief Generates a random double in the range [min, max]
     * @param min Minimum value (inclusive)
     * @param max Maximum value (inclusive)
     * @return double Random double in range
     */
    double randDouble(double min, double max);

    /**
     * @brief Generates a random boolean with given probability
     * @param probability Probability of returning true (0.0 to 1.0)
     * @return bool True with given probability, false otherwise
     */
    bool randBool(double probability);

    /**
     * @brief Reseeds the generator with new random value
     *
     * Useful if you suspect the generator state has been compromised
     * or you need a fresh sequence.
     */
    void reseed();

    /**
     * @brief Reseeds the generator with specific value
     * @param seed Seed value to use
     */
    void reseed(uint64_t seed);


};


#endif //KASYNO_RNG_H