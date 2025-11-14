//
// Created by moskw on 08.10.2025.
//

#ifndef KASYNO_ROUNDUI_H
#define KASYNO_ROUNDUI_H
#include <string>
#include <vector>

#include "FileHandler.h"


class RoundUI {
    static std::string trim(std::string str);
    int consoleWidth() const;
    std::string centerText(const std::string& text, int width) const;

    static void moveCursorToCenterRow();
    int displayWidthUtf8(const std::string& str) const;
public:
    RoundUI();
    ~RoundUI();

    void drawBox(const std::string& title, const std::vector<std::string>& lines,
            int padding = 1) const;
    void print(const std::string& text) const;
    int askChoice(const std::string& prompt, const std::vector<std::string>& options, bool clearScreen = true);
    int askInput(const std::string& prompt, int min, int max);
    std::string askInput(const std::string& prompt, const std::vector<std::string>& validInputs);
    void waitForKey(const std::string& messaege = "Press any key to continue...");
    void waitForEnter(const std::string& message = "Press ENTER to continue...") const;
    std::string ask(const std::string& prompt);
    void renderSlots(const std::vector<std::string>& symbols);
    static void pause(int ms);
    static void clear();
    void leaderboard(const std::string& title, const std::vector<LeaderboardEntry>& entries);
};


#endif //KASYNO_ROUNDUI_H