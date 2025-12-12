/**
 * @file Casino.h
 * @brief Main casino controller class that manages the application flow
 * @author Marczelloo
 * @date 2025-12-11
 */

//
// Created by moskw on 09.10.2025.
//

#ifndef KASYNO_CASINO_H
#define KASYNO_CASINO_H
#include <memory>

#include "Resources/Enums.h"
#include "Games/Game.h"
#include "Player.h"
#include "Rng.h"
#include "RoundUI.h"


/**
 * @class Casino
 * @brief Main controller for the casino application
 *
 * Manages the entire application lifecycle including:
 * - State management (main menu, casino menu, game menu)
 * - Player management
 * - Game selection and initialization
 * - Leaderboard operations
 */
class Casino {
    std::unique_ptr<Player> player;          ///< Current player instance
    std::unique_ptr<Game> game;              ///< Current game instance
    RoundUI ui;              ///< User interface handler
    Rng random;             ///< Random number generator
    GameState state;         ///< Current application state

    /**
     * @brief Validates player exists before game operations
     * @return bool True if player is valid, false otherwise
     */
    bool validatePlayer();
public:
    /**
     * @brief Default constructor - initializes casino with no player
     */
    Casino();

    /**
     * @brief Destructor - cleans up allocated resources
     */
    ~Casino() = default;

    Casino(const Casino&) = delete;
    Casino& operator=(const Casino&) = delete;

    Casino(Casino&&) noexcept = default;
    Casino& operator=(Casino&&) noexcept = default;

    /**
     * @brief Main application loop
     *
     * Runs the casino application, handling state transitions between menus
     */
    void run();

    /**
     * @brief Handles the main menu logic
     * @return GameState Next state to transition to
     */
    GameState handleMainMenu();

    /**
     * @brief Handles the casino menu logic
     * @return GameState Next state to transition to
     */
    GameState handleCasinoMenu();

    /**
     * @brief Handles the game selection menu logic
     * @return GameState Next state to transition to
     */
    GameState handleGameMenu();

    /**
     * @brief Creates a new player with name and initial balance
     * @return CreatePlayerResult Result of the player creation attempt
     */
    CreatePlayerResult createPlayer();

    /**
     * @brief Displays the leaderboard with top players
     */
    void checkLeaderboard();

    /**
     * @brief Exits the casino, saving player data to leaderboard
     */
    void exitCasino();

};


#endif //KASYNO_CASINO_H