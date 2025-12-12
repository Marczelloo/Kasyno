/**
 * @file FileHandler.h
 * @brief File operations handler for leaderboard management
 * @author Marczelloo
 * @date 2025-12-11
 */

//
// Created by moskw on 12.11.2025.
//

#ifndef KASYNO_FILEHANDLER_H
#define KASYNO_FILEHANDLER_H

#include <string>
#include <vector>

/**
 * @struct LeaderboardEntry
 * @brief Represents a single entry in the leaderboard
 */
struct LeaderboardEntry {
    std::string name;  ///< Player name
    int balance;       ///< Player's final balance
};

/**
 * @class FileHandler
 * @brief Handles file operations for leaderboard persistence
 *
 * Provides static methods for:
 * - Saving and loading leaderboard data
 * - Adding new entries
 * - Checking player existence
 */
class FileHandler {
private:
    /**
     * @brief Checks if a file exists
     * @param filename Name of file to check
     * @return bool True if file exists, false otherwise
     */
    static bool fileExists(const std::string& filename);
public:
    /**
     * @brief Saves entire leaderboard to file
     * @param entries Vector of leaderboard entries to save
     * @param filename Name of file to save to (default: "leaderboard.txt")
     * @return bool True if save was successful, false otherwise
     */
    static bool saveLeaderboard(const std::vector<LeaderboardEntry>& entries, const std::string& filename = "leaderboard.txt");

    /**
     * @brief Adds a new entry to the leaderboard
     * @param entry Leaderboard entry to add
     * @param filename Name of file to add to (default: "leaderboard.txt")
     * @return bool True if add was successful, false otherwise
     */
    static bool addEntry(const LeaderboardEntry& entry, const std::string& filename = "leaderboard.txt");

    /**
     * @brief Loads leaderboard from file
     * @param filename Name of file to load from (default: "leaderboard.txt")
     * @return std::vector<LeaderboardEntry> Vector of leaderboard entries
     */
    static std::vector<LeaderboardEntry> loadLeaderboard(const std::string& filename = "leaderboard.txt");

    /**
     * @brief Checks if a player with given name exists in leaderboard
     * @param playerName Name of player to check
     * @return bool True if player exists, false otherwise
     */
    static bool playerExists(const std::string& playerName);
};


#endif //KASYNO_FILEHANDLER_H