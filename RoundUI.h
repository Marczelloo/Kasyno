/**
 * @file RoundUI.h
 * @brief User interface handler for the casino application
 * @author Marczelloo
 * @date 2025-12-11
 */

//
// Created by moskw on 08.10.2025.
//

#ifndef KASYNO_ROUNDUI_H
#define KASYNO_ROUNDUI_H
#include <string>
#include <vector>

#include "FileHandler.h"
#include "Games/RouletteTypes.h"

/**
 * @class RoundUI
 * @brief Handles all user interface operations and rendering
 *
 * Provides methods for:
 * - Drawing boxes and frames
 * - Colorizing text with ANSI codes
 * - Getting user input
 * - Rendering game-specific interfaces (slots, roulette wheel)
 * - Displaying leaderboards
 */
class RoundUI {
    /**
     * @brief Trims whitespace from a string
     * @param str String to trim
     * @return std::string Trimmed string
     */
    static std::string trim(const std::string& str);

    /**
     * @brief Gets the current console width
     * @return int Console width in characters
     */
    int consoleWidth() const;

    /**
     * @brief Centers text within a given width
     * @param text Text to center
     * @param width Width to center within
     * @return std::string Centered text with padding
     */
    std::string centerText(const std::string& text, int width) const;

    /**
     * @brief Centers colored text (accounting for ANSI codes)
     * @param s Text with ANSI color codes
     * @param termWidth Terminal width
     * @return std::string Centered colored text
     */
    std::string centerColored(const std::string& s, int termWidth) const;

    /**
     * @brief Applies ANSI color codes to text
     * @param text Text to colorize
     * @param fg Foreground color code
     * @param bg Background color code
     * @param bold Whether to apply bold styling
     * @return std::string Colorized text with ANSI codes
     */
    static std::string colorize(const std::string& text, const std::string& fg, const std::string& bg, bool bold);

    /**
     * @brief Moves cursor to center row of console
     */
    static void moveCursorToCenterRow();

    /**
     * @brief Strips ANSI escape codes from a string
     * @param s String with ANSI codes
     * @return std::string String without ANSI codes
     */
    std::string stripAnsi(const std::string& s) const;

    /**
     * @brief Calculates display width of UTF-8 string
     * @param str UTF-8 encoded string
     * @return int Display width in characters
     */
    int displayWidthUtf8(const std::string& str) const;

    /**
     * @brief Validates if string is a valid integer
     * @param str String to validate
     * @return bool True if valid integer
     */
    static bool isValidInteger(const std::string& s);

    /**
     * @brief Safely converts string to integer with range check
     * @param str String to convert
     * @param value Output integer value
     * @param min Minimum allowed value
     * @param max Maximum allowed value
     * @return bool True if conversion successful and in range
     */
    static bool safeStringToInt(const std::string& s, int& value, int min, int max);
public:
    /**
     * @brief Default constructor
     */
    RoundUI() = default;

    /**
     * @brief Destructor
     */
    ~RoundUI() = default;

    RoundUI(const RoundUI&) = delete;
    RoundUI& operator=(const RoundUI&) = delete;

    /**
     * @brief Draws a box with title and content lines
     * @param title Box title
     * @param lines Content lines to display in box
     * @param padding Padding around content (default: 1)
     */
    void drawBox(const std::string& title, const std::vector<std::string>& lines,
            int padding = 1) const;

    /**
     * @brief Prints text to console
     * @param text Text to print
     */
    void print(const std::string& text) const;

    /**
     * @brief Asks user to choose from a list of options
     * @param prompt Prompt message
     * @param options List of options to choose from
     * @param clearScreen Whether to clear screen before displaying (default: true)
     * @return int Index of selected option
     */
    int askChoice(const std::string& prompt, const std::vector<std::string>& options, bool clearScreen = true) const;

    /**
     * @brief Asks user for numeric input within a range
     * @param prompt Prompt message
     * @param min Minimum valid value
     * @param max Maximum valid value
     * @return int User's input value
     */
    int askInput(const std::string& prompt, int min, int max) const;

    /**
     * @brief Asks user for string input from valid options
     * @param prompt Prompt message
     * @param validInputs List of valid string inputs
     * @return std::string User's input
     */
    std::string askInput(const std::string& prompt, const std::vector<std::string>& validInputs) const;

    /**
     * @brief Waits for any key press
     * @param message Message to display (default: "Press any key to continue...")
     */
    void waitForKey(const std::string& messaege = "Press any key to continue...") const;

    /**
     * @brief Waits for Enter key press
     * @param message Message to display (default: "Press ENTER to continue...")
     */
    void waitForEnter(const std::string& message = "Press ENTER to continue...") const;

    /**
     * @brief Asks user for string input
     * @param prompt Prompt message
     * @return std::string User's input
     */
    std::string ask(const std::string& prompt) const;

    /**
     * @brief Renders slot machine reels
     * @param symbols Vector of symbol strings to display
     */
    void renderSlots(const std::vector<std::string>& symbols);

    /**
     * @brief Renders roulette wheel with highlighted result
     * @param wheel Vector of roulette tiles
     * @param spunTile Index of the result tile to highlight
     */
    void renderWheel(const std::vector<RouletteTile>& wheel, const int &spunTile);

    /**
     * @brief Pauses execution for specified milliseconds
     * @param ms Milliseconds to pause
     */
    static void pause(int ms);

    /**
     * @brief Clears the console screen
     */
    static void clear();

    /**
     * @brief Displays leaderboard with entries
     * @param title Leaderboard title
     * @param entries Vector of leaderboard entries to display
     */
    void leaderboard(const std::string& title, const std::vector<LeaderboardEntry>& entries);
};


#endif //KASYNO_ROUNDUI_H