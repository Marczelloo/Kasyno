//
// Created by moskw on 12.11.2025.
//

#ifndef KASYNO_ROULETTEGAME_H
#define KASYNO_ROULETTEGAME_H
#include "Game.h"
#include "RouletteTypes.h"

enum class RouletteBetType {
    BET_RED = 0,
    BET_BLACK,
    BET_GREEN,
    BET_NUMBER,
    BET_ODD,
    BET_EVEN,
    BET_LOW,
    BET_HIGH,
};

class RouletteGame: public Game {
private:
    int askForBet(int maxBalance) override;
    int renderInterface(const Player& player) override;
    int calculateScore();
    void displayPayouts() const override;
    int lastScore = -1;
    std::string errorMessage;

    std::vector<RouletteTile> initWheel();
    RouletteTileType getColorForNumber(int number) const;
    int spinWheel();

    std::vector<RouletteTile> wheel;
    std::vector<RouletteTile> prevTiles;
    int spunTile = 0;


public:
    RouletteGame(Rng* rng);
    ~RouletteGame();
    GameState playRound(Player& player) override;
};


#endif //KASYNO_ROULETTEGAME_H