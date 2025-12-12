/**
 * @file RouletteGame.h
 * @brief European Roulette game implementation
 * @author Marczelloo
 * @date 2025-12-11
 */

//
// Created by moskw on 12.11.2025.
//

#ifndef KASYNO_ROULETTEGAME_H
#define KASYNO_ROULETTEGAME_H
#include "Game.h"
#include "RouletteTypes.h"

/**
 * @enum RouletteBetType
 * @brief Types of bets available in roulette
 */
enum class RouletteBetType {
    BET_RED = 0,     ///< Bet on red numbers
    BET_BLACK,       ///< Bet on black numbers
    BET_GREEN,       ///< Bet on green (0)
    BET_NUMBER,      ///< Bet on specific number
    BET_ODD,         ///< Bet on odd numbers
    BET_EVEN,        ///< Bet on even numbers
    BET_LOW,         ///< Bet on low numbers (1-18)
    BET_HIGH,        ///< Bet on high numbers (19-36)
};

/**
 * @class RouletteGame
 * @brief Implementation of European Roulette with 37 numbers (0-36)
 *
 * Features:
 * - Multiple bet types (color, number, odd/even, high/low)
 * - Animated wheel spin with progressive slowdown
 * - Different payouts for different bet types
 * - Visual wheel representation
 */
class RouletteGame: public Game {
private:
    int lastScore;                ///< Last round's score
    RouletteBetType betType;      ///< Current bet type
    int betNumber;                ///< Bet number (if betting on specific number)
    std::vector<RouletteTile> wheel;      ///< Roulette wheel tiles
    std::vector<RouletteTile> prevTiles;  ///< Previous tiles for animation
    int spunTile;                         ///< Result tile index

    /**
     * @brief Asks for bet with roulette-specific options
     * @param player Reference to current player
     * @return int Bet amount (or -1 if cancelled)
     */
    int askForBet(Player& player) override;

    /**
     * @brief Renders roulette game interface
     * @param player Current player
     * @return int Selected option index
     */
    int renderInterface(const Player& player) override;

    /**
     * @brief Calculates win multiplier based on bet type and result
     * @param selectedTile Index of the result tile
     * @return double Win multiplier (0.0 if lost)
     */
    double calculateMultiplier(int selectedTile);

    /**
     * @brief Displays roulette payout table
     */
    void displayPayouts() const override;

    /**
     * @brief Animates the wheel spinning
     * @param player Current player
     * @param resultIndex Final result index
     */
    void animateSpin(const Player& player, int resultIndex);

    /**
     * @brief Initializes the roulette wheel with 37 tiles
     * @return std::vector<RouletteTile> Initialized wheel
     */
    std::vector<RouletteTile> initWheel();

    /**
     * @brief Gets the color for a given number
     * @param number Number on the wheel (0-36)
     * @return RouletteTileType Color of the number
     */
    RouletteTileType getColorForNumber(int number) const;

    /**
     * @brief Spins the wheel and returns result index
     * @return int Index of the result tile
     */
    int spinWheel();
public:
    /**
     * @brief Constructor
     * @param rng Reference to random number generator
     */
    explicit RouletteGame(Rng &rng);

    /**
     * @brief Destructor
     */
    ~RouletteGame();

    /**
     * @brief Main game loop for roulette
     * @param player Current player
     * @return GameState Next state to transition to
     */
    GameState playRound(Player& player) override;
};


#endif //KASYNO_ROULETTEGAME_H