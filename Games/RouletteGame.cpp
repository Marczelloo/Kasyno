//
// Created by moskw on 12.11.2025.
//

#include "RouletteGame.h"
#include <array>
#include <algorithm>

RouletteGame::RouletteGame(Rng* rng): Game("Roulette", rng),
                            wheel(initWheel()),
                            spunTile(0),
                            lastScore(-1) {};

RouletteGame::~RouletteGame() = default;

RouletteTileType RouletteGame::getColorForNumber(int number) const {
    if (number == 0) return RouletteTileType::GREEN;

    static const std::array<int, 18> redNumbers = {
        1, 3, 5, 7, 9, 12, 14, 16, 18,
        19, 21, 23, 25, 27, 30, 32, 34, 36
    };

    if (std::find(redNumbers.begin(), redNumbers.end(), number) != redNumbers.end()) {
        return RouletteTileType::RED;
    } else {
        return RouletteTileType::BLACK;
    }
}

std::vector<RouletteTile> RouletteGame::initWheel() {
    static const std::vector<int> numbers = {
        0, 32, 15, 19, 4, 21, 2, 25,
        17, 34, 6, 27, 13, 36, 11, 30,
        8, 23, 10, 5, 24, 16, 33, 1,
        20, 14, 31, 9, 22, 18, 29, 7,
        28, 12, 35, 3, 26
    };

    std::vector<RouletteTile> wheel;
    wheel.reserve(numbers.size());

    for (int n : numbers) {
        wheel.push_back(RouletteTile{
            getColorForNumber(n),
            n
        });
    }

    return wheel;
}

int RouletteGame::spinWheel() {
    //placeholder for spinWheel
    ui.print("Placeholder for spinWheel");
    return 0;
}

int RouletteGame::askForBet(int maxBalance) {
    //placeholder for askForBet
    ui.print("Placeholder for askForBet");
    return 0;
}

int RouletteGame::calculateScore() {
    // Placeholder for score calculation logic
    ui.print("Placeholder for score calculation");
    return 0;
}

void RouletteGame::displayPayouts() const {
    //placeholder for payouts
    ui.print("Payouts placeholder");
}

int RouletteGame::renderInterface(const Player &player) {
    RoundUI::clear();

    ui.renderWheel(wheel, spunTile);

    std::vector<std::string> info;
    info.push_back(player.getName() + "'s Balance: " + std::to_string(player.getBalance()));
    info.push_back("Current bet: " + std::to_string(player.getCurrentBet()) + " - ");

    if (lastScore >= 0) {
        if (lastScore > 0) {
            info.push_back("You won " + std::to_string(lastScore) + "!");
        } else {
            info.push_back("No win this time. Better luck next spin!");
        }
    }
    ui.drawBox("", info);

    int option = ui.askChoice("What would you like to do?",
                              TextRes::ROULETTE_GAME_OPTIONS,
                              false);

    return option;
}

GameState RouletteGame::playRound(Player &player) {

    GameState newState = GameState::GAME_MENU;
    exit = false;

    while (!exit) {
        int option = renderInterface(player);

        switch (static_cast<RouletteOptions>(option)) {
            case RouletteOptions::SPIN: {
                ui.print("Spin option test");
            }
            case RouletteOptions::CHANGE_BET: {
                ui.print("Change bet option test");
            }
            case RouletteOptions::VIEW_PAYOUTS: {
                ui.print("View payouts option test");
            }
            case RouletteOptions::EXIT_TO_GAME_MENU:
            case RouletteOptions::EXIT: {
                LeaderboardEntry entry{player.getName(), player.getBalance()};
                FileHandler::addEntry(entry);

                exit = true;

                newState = (option == static_cast<int>(RouletteOptions::EXIT_TO_GAME_MENU)) ?
                             GameState::GAME_MENU : GameState::EXIT;
                break;
            }
            default:
                errorMessage = "Invalid choice, please try again.";
                break;
        }
    }

    return newState;
}