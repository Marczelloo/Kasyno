//
// Created by moskw on 12.11.2025.
//

#ifndef KASYNO_BLACKJACKGAME_H
#define KASYNO_BLACKJACKGAME_H
#include "Game.h"

enum Suit {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};

struct Card {
    std::string rank;
    Suit suit;
    int value;
};

class BlackjackGame: public Game {
    int askForBet(int maxBalance) override;
    int renderInterface(const Player& player) override;
    int calculateScore(std::vector<Card> playersHand, int bet);
    void displayPayouts() const override;
    void displayRules() const;
    void renderRound(const Player& player, bool playerTurn, std::string winningInfo);
    void handleRound(Player& player);

    bool playerTurn(Player& player, size_t handIndex);

    std::vector<std::vector<Card>> deck;
    std::vector<std::vector<Card>> initializeDeck();
    std::vector<std::vector<Card>> shuffleDeck();
    Card drawCard();

    std::vector<std::vector<Card>> playerHand;
    std::vector<int> handBets;
    std::vector<Card> dealerHand;
    std::vector<bool> surrendered;

    std::string errorMessage;
    int lastScore;
public:
    BlackjackGame(Rng &rng);
    ~BlackjackGame();
    GameState playRound(Player& player) override;
};


#endif //KASYNO_BLACKJACKGAME_H