//
// Created by moskw on 08.10.2025.
//

#include "RoundUI.h"

#include <iostream>
#include <ostream>
#include <algorithm>
#include <limits>
#include <bits/this_thread_sleep.h>

#ifdef _WIN32
    #include <windows.h>
#endif

RoundUI::RoundUI() {}

RoundUI::~RoundUI() {}

int RoundUI::consoleWidth() const {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 80;
#else
    struct winsize w{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) {
        return w.ws_col;
    }
    return 80;
#endif
}

void RoundUI::moveCursorToCenterRow() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;

    int termWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    SHORT targetCol = (SHORT)(termWidth / 2);

    COORD pos = csbi.dwCursorPosition;
    pos.X = targetCol;
    SetConsoleCursorPosition(hOut, pos);
#endif
}

std::string RoundUI::centerText(const std::string& text, int width) const {
    int displayLen = displayWidthUtf8(text);
    if (displayLen >= width) return text;

    int totalPadding = width - displayLen;
    int left  = totalPadding / 2;
    int right = totalPadding - left;

    return std::string(left, ' ') + text + std::string(right, ' ');
}
void RoundUI::drawBox(const std::string& title,
                      const std::vector<std::string>& lines,
                      int padding) const {
    int width = consoleWidth();
    if (width < 20) width = 80;

    int maxContentWidth = width - 4;
    if (maxContentWidth < 20) maxContentWidth = 20;

    auto truncate = [this, maxContentWidth](const std::string& s) {
        // cutting by bytes is still ok-ish for you, but if you want,
        // you can also make this UTF-8 aware later
        if ((int)s.size() <= maxContentWidth) return s;
        if (maxContentWidth <= 3) return s.substr(0, maxContentWidth);
        return s.substr(0, maxContentWidth - 3) + "...";
    };

    std::vector<std::string> content;
    content.reserve(lines.size());
    for (const auto& l : lines) content.push_back(truncate(l));

    // use displayWidthUtf8 here:
    int boxWidth = displayWidthUtf8(truncate(title));
    for (const auto& l : content)
        boxWidth = std::max(boxWidth, displayWidthUtf8(l));

    boxWidth += padding * 2;

    std::string horizontal(boxWidth + 2, '-');
    std::string top    = "+" + horizontal + "+";
    std::string middle = "+" + horizontal + "+";
    std::string bottom = "+" + horizontal + "+";

    int termWidth = consoleWidth();
    auto centeredLine = [this, termWidth](const std::string& l) {
        return centerText(l, termWidth);   // now also UTF-8 aware
    };

    std::cout << centeredLine(top) << "\n";

    if (!title.empty()) {
        std::string t = truncate(title);
        int titleWidth = displayWidthUtf8(t);
        int innerPaddingLeft  = (boxWidth - titleWidth) / 2;
        int innerPaddingRight = boxWidth - titleWidth - innerPaddingLeft;
        if (innerPaddingLeft  < 0) innerPaddingLeft  = 0;
        if (innerPaddingRight < 0) innerPaddingRight = 0;

        std::string line =
            "| " + std::string(innerPaddingLeft, ' ') + t +
            std::string(innerPaddingRight, ' ') + " |";
        std::cout << centeredLine(line) << "\n";
        std::cout << centeredLine(middle) << "\n";
    }

    for (const auto& l : content) {
        int lineWidth = displayWidthUtf8(l);
        int spaces = boxWidth - lineWidth - padding;
        if (spaces < 0) spaces = 0;

        std::string line =
            "| " + std::string(padding, ' ') + l +
            std::string(spaces, ' ') + " |";
        std::cout << centeredLine(line) << "\n";
    }

    std::cout << centeredLine(bottom) << "\n";
}

std::string RoundUI::trim(std::string str) {
    auto first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    auto last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

void RoundUI::print(const std::string &text) const {
    std::cout << centerText(text, consoleWidth()) << std::endl;
}

int RoundUI::askChoice(const std::string &prompt, const std::vector<std::string> &options, bool clearScreen) {
    if (prompt.empty()) {
        std::cout << "Your prompt for choice windows is empty " << std::endl;
        return -1;
    }

    if (options.empty()) {
        std::cout << "Your choices are empty" << std::endl;
        return -1;
    }

    while (true) {
        std::vector<std::string> lines;
        lines.reserve(options.size());
        for (int i = 0; i < (int)options.size(); ++i) {
            lines.push_back(std::to_string(i + 1) + ". " + options[i]);
        }

        if (clearScreen) {
            clear();
        }
        drawBox(prompt, lines);

        std::string answer;
        int choice;

        RoundUI::moveCursorToCenterRow();
        if (!std::getline(std::cin, answer)) {
            std::cout << "\nInput closed\n";
            return -1;
        }

        answer = trim(answer);
        if (answer.empty()) continue;

        try {
            choice = std::stoi(answer);
        } catch (...) {
            std::cout << "Invalid option\n";
            continue;
        }

        if (choice < 1 || choice > (int)options.size()) {
            std::cout << "Invalid choice\n";
            continue;
        }
        return choice - 1;
    }
}

std::string RoundUI::ask(const std::string &prompt) {
    if (prompt.empty()) {
        std::cerr << "Error: Your prompt for ask is empty" << std::endl;
        return "";
    }

    std::string trimmed_prompt = trim(prompt);
    if (trimmed_prompt.empty()) {
        std::cerr << "Error: Prompt containts only white spaces" << std::endl;
        return "";
    }

    std::string answer;

    while (true) {
        clear();

        std::vector<std::string> lines;
        drawBox(trimmed_prompt, lines);

        RoundUI::moveCursorToCenterRow();
        if (!std::getline(std::cin, answer)) {
            std::cout << "\nInput closed\n";
            return "";
        }

        answer = trim(answer);
        if (answer.empty()) {
            std::cout << centerText("Answer cannot be empty", consoleWidth()) << std::endl;
            pause(1000);
            continue;
        }

        return answer;
    }
}

int RoundUI::displayWidthUtf8(const std::string& s) const {
    int width = 0;

    for (std::size_t i = 0; i < s.size(); ) {
        unsigned char c = static_cast<unsigned char>(s[i]);

        // ASCII
        if (c < 0x80) {
            width += 1;
            ++i;
            continue;
        }

        // Decode minimal UTF-8 → code point
        uint32_t cp = 0;
        std::size_t len = 0;

        if ((c & 0xE0) == 0xC0 && i + 1 < s.size()) {          // 2 bytes
            cp = (c & 0x1F) << 6 |
                 (static_cast<unsigned char>(s[i+1]) & 0x3F);
            len = 2;
        } else if ((c & 0xF0) == 0xE0 && i + 2 < s.size()) {   // 3 bytes
            cp = (c & 0x0F) << 12 |
                 (static_cast<unsigned char>(s[i+1]) & 0x3F) << 6 |
                 (static_cast<unsigned char>(s[i+2]) & 0x3F);
            len = 3;
        } else if ((c & 0xF8) == 0xF0 && i + 3 < s.size()) {   // 4 bytes
            cp = (c & 0x07) << 18 |
                 (static_cast<unsigned char>(s[i+1]) & 0x3F) << 12 |
                 (static_cast<unsigned char>(s[i+2]) & 0x3F) << 6 |
                 (static_cast<unsigned char>(s[i+3]) & 0x3F);
            len = 4;
        } else {
            // broken byte, skip
            ++i;
            continue;
        }

        // Zero-width stuff: variation selector, ZWJ
        if (cp == 0xFE0F || cp == 0x200D) {
            i += len;
            continue;
        }

        // Most emojis / pictograms → 2 columns
        if ( (cp >= 0x1F300 && cp <= 0x1FAFF) ||   // emoji blocks
             (cp >= 0x2600 && cp <= 0x26FF) ||     // misc symbols (☀, ☂, etc.)
             (cp >= 0x2700 && cp <= 0x27BF) ||     // dingbats (✂, ✨…)
             cp == 0x2B50                          // ⭐ specifically
           ) {
            width += 2;
           } else {
               width += 1;
           }

        i += len;
    }

    return width;
}


void RoundUI::renderSlots(const std::vector<std::string>& symbols) {
    clear();

    int termWidth = consoleWidth();
    if (termWidth < 20) termWidth = 80;

    const int innerWidth = 25;

    std::string horizontal(innerWidth + 2, '-');
    std::string top    = "+" + horizontal + "+";
    std::string middle = "+" + horizontal + "+";
    std::string bottom = "+" + horizontal + "+";

    auto centeredLine = [this, termWidth](const std::string& l) {
        return centerText(l, termWidth);
    };

    std::cout << centeredLine(top) << "\n";

    const std::string title = "=== SLOTS GAME ===";
    int titleLen = (int)title.size();

    int padLeftTitle  = (innerWidth - titleLen) / 2;
    int padRightTitle = innerWidth - titleLen - padLeftTitle;
    if (padLeftTitle  < 0) padLeftTitle  = 0;
    if (padRightTitle < 0) padRightTitle = 0;

    std::cout << centeredLine(
        "| " + std::string(padLeftTitle, ' ') + title +
        std::string(padRightTitle, ' ') + " |"
    ) << "\n";

    std::cout << centeredLine(middle) << "\n";

    std::string row;
    for (std::size_t i = 0; i < symbols.size(); ++i) {
        row += symbols[i];
        if (i + 1 < symbols.size()) row += "  ";
    }

    // Używamy displayWidthUtf8 (policz kolumny, nie bajty)
    int rowDisplayWidth = displayWidthUtf8(row);

    int padTotal = innerWidth - rowDisplayWidth;
    if (padTotal < 0) padTotal = 0;

    int padLeftRow  = padTotal / 2;
    int padRightRow = padTotal - padLeftRow;

    std::cout << centeredLine(
        "| " + std::string(padLeftRow, ' ') + row +
        std::string(padRightRow, ' ') + " |"
    ) << "\n";

    std::cout << centeredLine(bottom) << "\n";
}

void RoundUI::pause(const int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void RoundUI::clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void RoundUI::leaderboard(const std::string& title, const std::vector<LeaderboardEntry>& entries) {
    std::vector<std::string> lines;

    if (entries.empty()) {
        lines.push_back("");
        lines.push_back("No players yet!");
        lines.push_back("");
    } else {
        for (size_t i = 0; i < entries.size(); ++i) {
            std::string rank = std::to_string(i + 1) + ".";
            std::string name = entries[i].name;
            std::string balance = std::to_string(entries[i].balance) + "$";

            std::string line = rank + " " + name + " - " + balance;
            lines.push_back(line);
        }
    }

    drawBox(title, lines);
}

void RoundUI::waitForKey(const std::string& message) {
    print(message);
    std::cin.get();
}

void RoundUI::waitForEnter(const std::string& message) const {
    print(message);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string RoundUI::askInput(const std::string &prompt,
                              const std::vector<std::string> &validInputs) {
    std::string answer;

    while (true) {
        clear();
        std::vector<std::string> lines = { prompt };
        drawBox("", lines);

        RoundUI::moveCursorToCenterRow();
        std::cout << "> ";        if (!std::getline(std::cin, answer)) {
            std::cout << "\nInput closed\n";
            return "";
        }

        answer = trim(answer);
        if (std::find(validInputs.begin(), validInputs.end(), answer) != validInputs.end()) {
            return answer;
        } else {
            std::cout << centerText("Invalid input.", consoleWidth()) << std::endl;
            pause(1000);
        }
    }
}

int RoundUI::askInput(const std::string &prompt, int min, int max) {
    std::string answer;

    while (true) {
        clear();
        std::vector<std::string> lines = {
            prompt,
            "Range: " + std::to_string(min) + " - " + std::to_string(max)
        };
        drawBox("", lines);

        RoundUI::moveCursorToCenterRow();
        std::cout << "> ";        if (!std::getline(std::cin, answer)) {
            std::cout << "\nInput closed\n";
            return -1;
        }

        answer = trim(answer);
        try {
            int value = std::stoi(answer);
            if (value >= min && value <= max) {
                return value;
            } else {
                std::cout << centerText(
                    "Input must be between " + std::to_string(min) +
                    " and " + std::to_string(max),
                    consoleWidth()) << std::endl;
                pause(1000);
            }
        } catch (...) {
            std::cout << centerText("Invalid number.", consoleWidth()) << std::endl;
            pause(1000);
        }
    }
}









