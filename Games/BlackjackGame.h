/**
 * @file BlackjackGame.h
 * @brief Blackjack card game implementation
 * @author Marczelloo
 * @date 2025-12-11
 */

//
// Created by moskw on 12.11.2025.
//

#ifndef KASYNO_BLACKJACKGAME_H
#define KASYNO_BLACKJACKGAME_H
#include "Game.h"

/**
 * @enum Suit
 * @brief Card suits in a deck
 */
enum Suit {
    HEARTS,      ///< Hearts suit
    DIAMONDS,    ///< Diamonds suit
    CLUBS,       ///< Clubs suit
    SPADES       ///< Spades suit
};

/**
 * @struct Card
 * @brief Represents a playing card
 */
struct Card {
    std::string rank;  ///< Card rank (A, 2-10, J, Q, K)
    Suit suit;         ///< Card suit
    int value;         ///< Card value for blackjack (1-11)
};

/**
 * @class BlackjackGame
 * @brief Implementation of European Blackjack game
 *
 * Features:
 * - Hit, Stand, Double Down, Split, Surrender
 * - Multiple hands support after split
 * - Blackjack detection (3:2 payout)
 * - Dealer hits to 17
 */
class BlackjackGame: public Game {
private:
    std::vector<std::vector<Card>> playerHand;  ///< Player's hands (can be multiple after split)
    std::vector<int> handBets;                  ///< Bet for each hand
    std::vector<Card> dealerHand;               ///< Dealer's hand
    std::vector<bool> surrendered;              ///< Surrender status for each hand
    int lastScore;            ///< Last round's score
    std::vector<std::vector<Card>> deck;  ///< Deck of cards (multiple decks)


    /**
     * @brief Asks for bet with blackjack-specific options
     * @param maxBalance Maximum bet amount
     * @return int Bet amount
     */
    int askForBet(Player& player) override;

    /**
     * @brief Renders blackjack game interface
     * @param player Current player
     * @return int Selected option index
     */
    int renderInterface(const Player& player) override;

    /**
     * @brief Displays blackjack payout table
     */
    void displayPayouts() const override;

    /**
     * @brief Displays blackjack rules
     */
    void displayRules() const;

    /**
     * @brief Renders the current round state
     * @param player Current player
     * @param playerTurn Whether it's player's turn
     * @param winningInfo Information about wins/losses
     */
    void renderRound(const Player &player, bool playerTurn, const std::string &winningInfo) const;

    /**
   * @brief Handles a single blackjack round
   * @param player Current player
   * @return double Total payout multiplier (0.0 if lost)
   */
    double handleRound(Player& player);

    /**
     * @brief Handles player's turn for a specific hand
     * @param player Current player
     * @param handIndex Index of the hand being played
     * @return bool True if player didn't bust, false otherwise
     */
    bool playerTurn(Player& player, size_t handIndex);

    /**
     * @brief Initializes a standard 52-card deck
     * @return std::vector<std::vector<Card>> Initialized deck
     */
    static std::vector<std::vector<Card>> initializeDeck();

    /**
     * @brief Shuffles the deck
     * @return std::vector<std::vector<Card>> Shuffled deck
     */
    std::vector<std::vector<Card>> shuffleDeck();

    /**
     * @brief Draws a card from the deck
     * @return Card Drawn card
     */
    Card drawCard();

public:
    /**
     * @brief Constructor
     * @param rng Reference to random number generator
     */
    explicit BlackjackGame(Rng &rng);

    /**
     * @brief Destructor
     */
    ~BlackjackGame();

    /**
     * @brief Main game loop for blackjack
     * @param player Current player
     * @return GameState Next state to transition to
     */
    GameState playRound(Player& player) override;
};


#endif //KASYNO_BLACKJACKGAME_H