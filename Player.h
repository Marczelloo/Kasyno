/**
 * @file Player.h
 * @brief Player class representing a casino player
 * @author Marczelloo
 * @date 2025-12-11
 */

//
// Created by moskw on 08.10.2025.
//

#ifndef KASYNO_PLAYER_H
#define KASYNO_PLAYER_H
#include <string>

/**
 * @class Player
 * @brief Represents a player in the casino with balance and statistics
 *
 * Manages player information including:
 * - Name and balance
 * - Current bet amount
 * - Total winnings
 */
class Player {
    std::string name;        ///< Player's name
    int balance;             ///< Current balance in the casino
    int winnings;            ///< Total winnings accumulated
    int currentBet;          ///< Current bet amount
public:
    /**
     * @brief Default constructor - creates player with empty name and zero balance
     */
    Player();

    /**
     * @brief Constructor with parameters
     * @param name Player's name
     * @param balance Initial balance
     */
    Player(const std::string& name,const int& balance);

    /**
   * @brief Default destructor
   */
    ~Player() = default;

    Player(const Player&) = default;
    Player& operator=(const Player&) = default;
    Player(Player&&) noexcept = default;
    Player& operator=(Player&&) noexcept = default;

    /**
     * @brief Gets the player's name
     * @return std::string Player's name
     */
    std::string getName() const;

    /**
     * @brief Gets the player's current balance
     * @return int Current balance (always >= 0)
     */
    int getBalance() const;

    /**
     * @brief Gets the player's total winnings
     * @return int Total winnings (negative if net losses)
     */
    int getWinnings() const;

    /**
     * @brief Gets the player's current bet
     * @return int Current bet (0 if no active bet)
     */
    int getCurrentBet() const;

    /**
    * @brief Checks if player has an active bet
    * @return bool True if currentBet > 0
    */
    bool hasActiveBet() const;

    /**
     * @brief Checks if player can afford a bet
     * @param amount Bet amount to check
     * @return bool True if balance >= amount
     */
    bool canAffordBet(int amount) const;

    /**
    * @brief Sets player's name
    * @param newName New name
    * @throws std::invalid_argument if name is empty
    */
    void setName(const std::string& newName);

    /**
     * @brief Sets balance to specific value
     * @param newBalance New balance
     * @throws std::invalid_argument if balance is negative
     */
    void setBalance(const int& newBalance);

    /**
     * @brief Sets the player's total winnings
     * @param newWinnings New winnings value (can be negative)
     */
    void setWinnings(const int newWinnings);

    /**
     * @brief Sets current bet amount
     * @param newBet New bet amount
     * @throws std::invalid_argument if bet is negative or exceeds balance
     */
    void setCurrentBet(const int newBet);

    /**
     * @brief Places a bet
     * @param amount Bet amount
     * @throws std::invalid_argument if amount <= 0 or exceeds balance
     * @throws std::logic_error if player already has an active bet
     */
    void placeBet(int amount);

    /**
     * @brief Wins current bet with multiplier
     * @param multiplier Win multiplier (e.g., 2.0 for double)
     * @throws std::logic_error if no active bet
     * @throws std::invalid_argument if multiplier <= 0
     */
    void winBet(double multiplier);

    /**
     * @brief Cancels current bet and returns money
     * @throws std::logic_error if no active bet
     */
    void cancelBet();

    /**
     * @brief Loses current bet
     * @throws std::logic_error if no active bet
     */
    void loseBet();

    /**
     * @brief Updates balance by amount
     * @param amount Amount to add (can be negative)
     * @throws std::invalid_argument if result would be negative
     */
    void updateBalance(int amount);

    /**
     * @brief Resets player statistics but keeps balance
     */
    void resetStats();

    /**
     * @brief Resets player to initial state (Guest, 0 balance)
     */
    void reset();
};


#endif //KASYNO_PLAYER_H