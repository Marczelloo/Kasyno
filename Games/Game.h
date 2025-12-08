//
// Created by moskw on 08.10.2025.
//

#ifndef KASYNO_GAME_H
#define KASYNO_GAME_H
#include <string>

#include "../Player.h"
#include "../Rng.h"
#include "../RoundUI.h"
#include "../Resources/Enums.h"
#include "../Resources/TextRes.h"

//GAME_MENU -> PETLA GRY -> GAME_MENU
// playRound -> ask for bet -> game loop przerywany tylko koncem srodkow albo wybraniem opcji exit, return to main menu
// | interfejs gry|
// menu opcji gry (zmiana bettu, wyjscie do menu gry itp)

// w grze mamy menu z opcjami gry, akcji gry, zmiany bettu, wybrania innej gry wyjscia itp
// w slotsach np mozna dac opcje szybkiego bettowania 10, 20, 50 itp
// w slotsach chce rowniez dodac zmiane ilosci lini i kolumn
// w blackjacku mozna dac opcje double down, split itp
// kazda gra ma swoj unikalny zestaw opcji ktore sa dostepne w trakcie rundy

class Game {
protected:
    std::string name;
    Rng& random;
    RoundUI ui;
    bool exit = false;

    virtual int askForBet(int maxBalance) {
        if (maxBalance <= 0) {
            ui.print("No balance avaiable");
            return 0;
        }

        int choice = ui.askChoice(TextRes::BET_SELECT_TITLE, TextRes::BET_SELECT_OPTIONS);

        switch (static_cast<BetOptions>(choice)) {
            case BetOptions::BET_ALL_IN:
                return maxBalance;
            case BetOptions::BET_HALF:
                return maxBalance / 2;
            case BetOptions::BET_QUARTER:
                return maxBalance / 4;
            case BetOptions::BET_CUSTOM: {
                int bet = ui.askInput("Enter your bet amount (1 - " + std::to_string(maxBalance) + "): ", 1, maxBalance);
                return (bet > 0 && bet <= maxBalance) ? bet : maxBalance;
            }
            default:
                ui.print("Invalid choice, defaulting to custom amount!");
                int bet = ui.askInput("Enter your bet amount (1 - " + std::to_string(maxBalance) + "): ", 1, maxBalance);
                return (bet > 0 && bet <= maxBalance) ? bet : maxBalance;
        }
    }

    virtual int renderInterface(const Player& player) {
        ui.clear();
        ui.print("=== " + name + " GAME ===");
        ui.print("Current Player: " + player.getName());
        ui.print("Current Balance: " + std::to_string(player.getBalance()));
        ui.print("Current Bet: " + std::to_string(player.getCurrentBet()));
        ui.print("-----------------------");

        int option = ui.askChoice("Select an option:", {
            "Play Round",
            "Change Bet",
            "Exit to Game Menu",
            "Exit Casino"
        });

        return option;
    }

    virtual void displayPayouts() const {
        ui.clear();
        ui.print("Payouts information is not available for this game.");
        ui.waitForEnter("Press ENTER to return");
    }
public:
    Game(const std::string& gameName, Rng& rng): name(gameName), random(rng) {}
    virtual ~Game() = default;

    virtual GameState playRound(Player& player) = 0;

    std::string getName() const { return name; };
};


#endif //KASYNO_GAME_H