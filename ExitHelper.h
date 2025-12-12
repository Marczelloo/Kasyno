/**
 * @file ExitHelper.h
 * @brief Helper function for handling casino exit with save confirmation
 * @author Marczelloo
 * @date 2025-12-11
 */

//
// Created by moskw on 7.12.2025.
//

#ifndef KASYNO_EXITHELPER_H
#define KASYNO_EXITHELPER_H

#include "RoundUI.h"
#include "Player.h"
#include "FileHandler.h"

/**
 * @brief Confirms exit and saves player data to leaderboard
 *
 * Prompts the user to confirm exit and saves player data to leaderboard if confirmed.
 *
 * @param ui Reference to RoundUI for user interaction
 * @param player Reference to Player to save
 * @return bool True if user confirmed exit, false if cancelled
 */
inline bool confirmExitAndSave(RoundUI& ui, Player& player) {
    const int response = ui.askChoice(
        "Are you sure you want to exit the casino?",
        {"Yes", "No"}
    );

    if (response != 0) {
        ui.print("Exit cancelled.");
        return false;
    }

    ui.print("Exiting...");

    LeaderboardEntry entry{player.getName(), player.getBalance()};

    if (FileHandler::addEntry(entry)) {
        ui.print("Your progress has been saved to the leaderboard!");
    } else {
        ui.print("Warning: Failed to save your progress!");
    }

    return true;
}

#endif //KASYNO_EXITHELPER_H