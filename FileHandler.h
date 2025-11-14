//
// Created by moskw on 12.11.2025.
//

#ifndef KASYNO_FILEHANDLER_H
#define KASYNO_FILEHANDLER_H

#include <string>
#include <vector>

struct LeaderboardEntry {
    std::string name;
    int balance;
};


class FileHandler {
private:
    static bool fileExists(const std::string& filename);
public:
    static bool saveLeaderboard(const std::vector<LeaderboardEntry>& entries, const std::string& filename = "leaderboard.txt");
    static bool addEntry(const LeaderboardEntry& entry, const std::string& filename = "leaderboard.txt");
    static std::vector<LeaderboardEntry> loadLeaderboard(const std::string& filename = "leaderboard.txt");
    static bool playerExists(const std::string& playerName);
};


#endif //KASYNO_FILEHANDLER_H