//
// Created by moskw on 12.11.2025.
//

#include "FileHandler.h"
#include <fstream>
#include <vector>
#include <algorithm>


bool FileHandler::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

bool FileHandler::saveLeaderboard(const std::vector<LeaderboardEntry>& entries, const std::string& filename) {
    std::ofstream file(filename, std::ios::trunc);

    if (!file.is_open()) return false;

    for (const auto& entry : entries) {
        file << entry.name << "||" << entry.balance << "\n";
    }

    file.close();

    return true;
}

bool FileHandler::addEntry(const LeaderboardEntry& entry, const std::string& filename) {
    auto entries = loadLeaderboard(filename);

    auto it = std::find_if(entries.begin(), entries.end(),
        [&entry](const LeaderboardEntry& e) {
            return e.name == entry.name;
        });

    if (it != entries.end()) {
        it->balance = entry.balance;
    } else {
        entries.push_back(entry);
    }

    return saveLeaderboard(entries, filename);
}

std::vector<LeaderboardEntry> FileHandler::loadLeaderboard(const std::string& filename) {
    std::vector<LeaderboardEntry> entries;

    if (!fileExists(filename)) {
        std::ofstream createFile(filename, std::ios::trunc);
        createFile.close();
        return entries;
    }

    std::ifstream file(filename);

    if (!file.is_open()) return entries;

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find("||");
        if (pos != std::string::npos) {
            try {
                LeaderboardEntry entry;
                entry.name = line.substr(0, pos);
                entry.balance = std::stoi(line.substr(pos + 2));
                entries.push_back(entry);
            } catch(...) {
                continue;
            }
        }
    }

    file.close();

    std::sort(entries.begin(), entries.end(), [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
        return b.balance < a.balance;
    });

    return entries;
}

bool FileHandler::playerExists(const std::string& playerName) {
    auto entries = loadLeaderboard("leaderboard.txt");

    for (const auto& entry : entries) {
        if (entry.name == playerName) {
            return true;
        }
    }

    return false;
}