//
// Created by moskw on 09.10.2025.
//

#ifndef KASYNO_TEXTRES_H
#define KASYNO_TEXTRES_H
#include <string>
#include <vector>

namespace TextRes {
    constexpr const char* MAIN_MENU_TITLE = "CASINO MAIN MENU";
    constexpr const char* LEADERBOARD_TITLE = "TOP 100 LEADERBOARD";

    const std::vector<std::string> MAIN_MENU_OPTIONS = {
        "Create player",
        "Check leaderboard",
        "Exit",
    };

    constexpr const char* CASINO_TITLE = "CASINO";
    constexpr const char* CASINO_PLAYER = "Current player: ";
    constexpr const char* CASINO_BALANCE = "Current balance: ";

    const std::vector<std::string> CASINO_OPTIONS = {
        "Select game",
        "Check leaderboard",
        "Return to Main Menu",
        "Exit"
    };

    constexpr const char* GAME_SELECT_TITLE = "GAME SELECTION";

    const std::vector<std::string> GAME_SELECT_OPTIONS = {
        "SLOTS",
        "ROULETE",
        "BLACKJACK",
        "Return to Casino Menu",
        "Exit"
    };

    constexpr const char* BET_SELECT_TITLE = "SELECT YOUR BET";

    const std::vector<std::string> BET_SELECT_OPTIONS = {
        "All-in",
        "50% of balance",
        "25% of balance",
        "Custom amount"
    };

    const std::vector<std::string> SLOTS_GAME_OPTIONS = {
        "Spin",
        "Change Bet",
        "View payouts",
        "Exit to Game Menu",
        "Exit"
    };

    constexpr const char* SLOTS_BET_OPTIONS_TITLE = "SELECT YOUR SLOTS BET";

    const std::vector<std::string> SLOTS_BET_OPTIONS = {
        "10$", "20$", "50$", "100$", "200$", "500$"
    };

    const std::vector<std::string> SLOT_SYMBOLS = {
        "🍒", "🍋", "💎", "🌟", "🍀", "💰"
    };

    const std::vector<std::string> ROULETTE_GAME_OPTIONS = {
        "Spin the wheel",
        "Change Bet",
        "View payouts",
        "Exit to Game Menu",
        "Exit"
    };

    constexpr const char* ROULETTE_BET_OPTIONS_TITLE = "SELECT YOUR ROULETTE BET";

    const std::vector<std::string> ROULETTE_BET_TYPES = {
        "Red",
        "Black",
        "Green (0)",
        "Specific Number (0-36)",
        "Odd",
        "Even",
        "Low (1-18)",
        "High (19-36)"
    };

    constexpr const char* BLACKJACK_BET_OPTIONS_TITLE = "SELECT YOUR BLACKJACK BET";

    const std::vector<std::string> BLACKJACK_GAME_OPTIONS = {
        "Play Round",
        "Change Bet",
        "View payouts",
        "Rules",
        "Exit to Game Menu",
        "Exit"
    };

    constexpr const char* BLACKJACK_ROUND_OPTIONS_TITLE = "SELECT YOUR ACTION";

    const std::vector<std::string> BLACKJACK_ROUND_OPTIONS = {
        "HIT",
        "STAND",
        "DOUBLE DOWN",
        "SPLIT",
        "SURRENDER"
    };
}

#endif //KASYNO_TEXTRES_H