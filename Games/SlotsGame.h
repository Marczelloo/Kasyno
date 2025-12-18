/**
 * @file SlotsGame.h
 * @brief Slot machine game implementation
 * @author Marczelloo
 * @date 2025-12-11
 */

//
// Created by moskw on 12.11.2025.
//

#ifndef KASYNO_SLOTSGAME_H
#define KASYNO_SLOTSGAME_H
#include "Game.h"
#include <array>

/**
 * @class SlotsGame
 * @brief Implementation of a 3-reel slot machine
 *
 * Features:
 * - 3 reels with 6 different symbols
 * - Payouts for pairs and triples
 * - Animated reel spinning
 * - Quick bet options
 */
class SlotsGame: public Game {
private:
    /**
     * @brief Asks for bet with quick bet buttons
     * @param player Current player
     * @return int Bet amount
     */
    int askForBet(Player& player) override;

    /**
     * @brief Spins the slot machine reels
     * @return std::array<int, 3> Result symbols for each reel
     */
    std::array<int, 3> spinSlots();

    /**
     * @brief Renders slots game interface
     * @param player Current player
     * @return int Selected option index
     */
    int renderInterface(const Player& player) override;

    /**
     * @brief Calculates win multiplier based on slot results
     * @param slots Array of symbol indices
     * @return double Win multiplier (0.0 if lost)
     */
    double calculateMultiplier(const std::array<int, 3>& slots);

    /**
     * @brief Displays slots payout table
     */
    void displayPayouts() const override;

    /**
     * @brief Animates the reels spinning
     * @param player Current player
     * @param finalSlots Final symbol results
     */
    void animateSpin(const Player& player, const std::array<int, 3>& finalSlots);

    std::array<int, 3> slots = {-1, -1, -1};  ///< Current slot symbols
    int lastScore = -1;                        ///< Last round's score
    const int tripletPayouts[6] = {3, 5, 10, 20, 50, 100};  ///< Payouts for three matching symbols
    const int pairPayouts[6] = {1, 1, 2, 2, 3, 5};          ///< Payouts for two matching symbols
public:
    /**
     * @brief Constructor
     * @param rng Reference to random number generator
     */
    explicit SlotsGame(Rng &rng);

    /**
     * @brief Destructor
     */
    ~SlotsGame();

    /**
     * @brief Main game loop for slots
     * @param player Current player
     * @return GameState Next state to transition to
     */
    GameState playRound(Player& player) override;
};


#endif //KASYNO_SLOTSGAME_H