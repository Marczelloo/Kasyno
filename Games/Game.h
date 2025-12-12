/**
 * @file Game.h
 * @brief Abstract base class for all casino games
 * @author Marczelloo
 * @date 2025-12-11
 */

//
// Created by moskw on 08.10.2025.
//

#ifndef KASYNO_GAME_H
#define KASYNO_GAME_H
#include <stdexcept>
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

/**
 * @class Game
 * @brief Abstract base class defining the interface for all casino games
 *
 * Provides common functionality for all games:
 * - Betting system
 * - Game interface rendering
 * - Payout information display
 *
 * Each concrete game must implement the playRound() method.
 */
class Game {
protected:
    std::string name;        ///< Name of the game
    Rng& random;            ///< Reference to random number generator
    RoundUI ui;             ///< User interface handler
    bool exit = false;      ///< Flag indicating if user wants to exit
    std::string errorMessage;///< Stores error messages to display to user

    /**
     * @brief Asks player for bet amount with quick options
     * @param player Reference to current player
     * @return int Bet amount (or -1 if cancelled/invalid)
     * @note This is a default implementation - games can override it
     */
    virtual int askForBet(Player& player) {
        int maxBalance = player.getBalance();

        if (maxBalance <= 0) {
            ui.print("No balance available");
            ui.waitForEnter();
            return -1;
        }

        int choice = ui.askChoice(TextRes::BET_SELECT_TITLE, TextRes::BET_SELECT_OPTIONS);

        int betAmount = 0;

        switch (static_cast<BetOptions>(choice)) {
            case BetOptions::BET_ALL_IN:
                betAmount = maxBalance;
                break;
            case BetOptions::BET_HALF:
                betAmount = maxBalance / 2;
                break;
            case BetOptions::BET_QUARTER:
                betAmount = maxBalance / 4;
                break;
            case BetOptions::BET_CUSTOM: {
                betAmount = ui.askInput(
            "Enter your bet amount (1 - " + std::to_string(maxBalance) + "): ",
                1,
                    maxBalance
                );
                break;
            }
            default:
                ui.print("Invalid choice, defaulting to custom amount!");
                betAmount = ui.askInput(
                    "Enter your bet amount (1 - " + std::to_string(maxBalance) + "): ",
                    1,
                    maxBalance
                );
                break;
        }

        if (betAmount <= 0 || !player.canAffordBet(betAmount)) {
            errorMessage = "Insufficient balance for this bet!";
            return askForBet(player);
        }

        return betAmount;
    }

    /**
     * @brief Renders the game interface with player info and options
     * @param player Current player
     * @return int Index of selected option
     */
    virtual int renderInterface(const Player& player) {
        RoundUI::clear();
        ui.print("=== " + name + " GAME ===");
        ui.print("Current Player: " + player.getName());
        ui.print("Current Balance: " + std::to_string(player.getBalance()));

        if (player.hasActiveBet()) {
            ui.print("Current Bet: " + std::to_string(player.getCurrentBet()));
        }

        ui.print("-----------------------");

        int option = ui.askChoice("Select an option:", {
            "Play Round",
            "Change Bet",
            "Exit to Game Menu",
            "Exit Casino"
        });

        return option;
    }

    /**
     * @brief Displays payout information for the game
     */
    virtual void displayPayouts() const {
        RoundUI::clear();
        ui.print("Payouts information is not available for this game.");
        ui.waitForEnter("Press ENTER to return");
    }
public:
    /**
     * @brief Constructor
     * @param gameName Name of the game
     * @param rng Reference to random number generator
     */
    Game(const std::string& gameName, Rng& rng): name(gameName), random(rng) {
        if (gameName.empty()) {
            throw std::invalid_argument("Game::game name cannot be empty");
        }
    }

    /**
     * @brief Virtual destructor
     */
    virtual ~Game() = default;

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    Game(Game&&) noexcept = default;
    Game& operator=(Game&&) noexcept = default;

    /**
     * @brief Main game loop - must be implemented by derived classes
     * @param player Reference to current player
     * @return GameState State to transition to after game ends
     */
    virtual GameState playRound(Player& player) = 0;

    /**
     * @brief Gets the name of the game
     * @return std::string Game name
     */
    std::string getName() const { return name; };
};


#endif //KASYNO_GAME_H