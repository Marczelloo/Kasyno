/**
 * @file TextRes.h
 * @brief Text resource definitions for UI strings
 * @author Marczelloo
 * @date 2025-12-11
 */

//
// Created by moskw on 09.10.2025.
//

#ifndef KASYNO_TEXTRES_H
#define KASYNO_TEXTRES_H
#include <string>
#include <vector>

/**
 * @namespace TextRes
 * @brief Contains all text resources and UI strings used in the application
 *
 * Centralizes all user-facing text for easy modification and localization.
 */
namespace TextRes {
    // Main Menu
    constexpr const char* MAIN_MENU_TITLE = "CASINO MAIN MENU";  ///< Main menu title
    constexpr const char* LEADERBOARD_TITLE = "TOP 100 LEADERBOARD";  ///< Leaderboard title

    const std::vector<std::string> MAIN_MENU_OPTIONS = {  ///< Main menu option texts
        "Create player",
        "Check leaderboard",
        "Exit",
    };

    // Casino Menu
    constexpr const char* CASINO_TITLE = "CASINO";  ///< Casino menu title
    constexpr const char* CASINO_PLAYER = "Current player: ";  ///< Player name label
    constexpr const char* CASINO_BALANCE = "Current balance: ";  ///< Balance label

    const std::vector<std::string> CASINO_OPTIONS = {  ///< Casino menu option texts
        "Select game",
        "Check leaderboard",
        "Return to Main Menu",
        "Exit"
    };

    // Game Selection
    constexpr const char* GAME_SELECT_TITLE = "GAME SELECTION";  ///< Game selection title

    const std::vector<std::string> GAME_SELECT_OPTIONS = {  ///< Game selection option texts
        "SLOTS",
        "ROULETE",
        "BLACKJACK",
        "Return to Casino Menu",
        "Exit"
    };

    // Betting
    constexpr const char* BET_SELECT_TITLE = "SELECT YOUR BET";  ///< Bet selection title

    const std::vector<std::string> BET_SELECT_OPTIONS = {  ///< General bet option texts
        "All-in",
        "50% of balance",
        "25% of balance",
        "Custom amount"
    };

    // Slots Game
    const std::vector<std::string> SLOTS_GAME_OPTIONS = {  ///< Slots game menu options
        "Spin",
        "Change Bet",
        "View payouts",
        "Exit to Game Menu",
        "Exit"
    };

    constexpr const char* SLOTS_BET_OPTIONS_TITLE = "SELECT YOUR SLOTS BET";  ///< Slots bet selection title

    const std::vector<std::string> SLOTS_BET_OPTIONS = {  ///< Quick bet amounts for slots
        "10$", "20$", "50$", "100$", "200$", "500$"
    };

    const std::vector<std::string> SLOT_SYMBOLS = {  ///< Slot machine symbols (emojis)
        "🍒", "🍋", "💎", "🌟", "🍀", "💰"
    };

    // Roulette Game
    const std::vector<std::string> ROULETTE_GAME_OPTIONS = {  ///< Roulette game menu options
        "Spin the wheel",
        "Change Bet",
        "View payouts",
        "Exit to Game Menu",
        "Exit"
    };

    constexpr const char* ROULETTE_BET_OPTIONS_TITLE = "SELECT YOUR ROULETTE BET";  ///< Roulette bet selection title

    const std::vector<std::string> ROULETTE_BET_TYPES = {  ///< Roulette bet type options
        "Red",
        "Black",
        "Green (0)",
        "Specific Number (0-36)",
        "Odd",
        "Even",
        "Low (1-18)",
        "High (19-36)"
    };

    // Blackjack Game
    constexpr const char* BLACKJACK_BET_OPTIONS_TITLE = "SELECT YOUR BLACKJACK BET";  ///< Blackjack bet selection title

    const std::vector<std::string> BLACKJACK_GAME_OPTIONS = {  ///< Blackjack game menu options
        "Play Round",
        "Change Bet",
        "View payouts",
        "Rules",
        "Exit to Game Menu",
        "Exit"
    };

    constexpr const char* BLACKJACK_ROUND_OPTIONS_TITLE = "SELECT YOUR ACTION";  ///< Blackjack action selection title

    const std::vector<std::string> BLACKJACK_ROUND_OPTIONS = {
        "HIT",
        "STAND",
        "DOUBLE DOWN",
        "SPLIT",
        "SURRENDER"
    };
}

#endif //KASYNO_TEXTRES_H