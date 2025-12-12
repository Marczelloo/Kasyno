//
// Created by moskw on 08.10.2025.
//

#include "RoundUI.h"

#include <iostream>
#include <ostream>
#include <algorithm>
#include <limits>
#include <unordered_map>
#include <bits/this_thread_sleep.h>
#include "Games/RouletteTypes.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

static const std::unordered_map<std::string, std::string> ansiMap = {
    {"reset", "0"},
    {"bold", "1"},
    {"black", "30"}, {"red", "31"},    {"green", "32"},
    {"yellow","33"}, {"blue","34"},    {"magenta","35"},
    {"cyan","36"},  {"white","37"},
    // tła
    {"bg_black","40"}, {"bg_red","41"}, {"bg_green","42"},
    {"bg_yellow","43"},{"bg_blue","44"},{"bg_magenta","45"},
    {"bg_cyan","46"},{"bg_white","47"}
};

static inline std::string ansi(const std::string& code) {
    return "\x1b[" + code + "m";
};

std::string RoundUI::trim(const std::string& str) {
    if (str.empty()) return "";

    auto first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";

    auto last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

bool RoundUI::isValidInteger(const std::string& str) {
    if (str.empty()) return false;

    size_t start = 0;
    if (str[0] == '-' || str[0] == '+') {
        if (str.size() == 1) return false;
        start = 1;
    }

    for (size_t i = start; i < str.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(str[i]))) {
            return false;
        }
    }

    return true;
}

bool RoundUI::safeStringToInt(const std::string& str, int& value, int min, int max) {
    if (!isValidInteger(str)) return false;

    try {
        size_t pos = 0;
        long long ll = std::stoll(str, &pos);

        if (pos != str.size()) return false;

        if (ll < std::numeric_limits<int>::min() ||
            ll > std::numeric_limits<int>::max()) {
            return false;
            }

        value = static_cast<int>(ll);
        return value >= min && value <= max;

    } catch (const std::out_of_range&) {
        return false;
    } catch (const std::invalid_argument&) {
        return false;
    }
}

static bool enableAnsiColors() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return false;
    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode)) return false;
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    return SetConsoleMode(hOut, mode) != 0;
#else
    return true;
#endif
}

std::string RoundUI::colorize(const std::string& text, const std::string& fg= "", const std::string& bg = "", bool bold = false) {
    std::string seq;

    if (bold) seq += ansiMap.at("bold") + ";";
    if (!fg.empty() &&  ansiMap.count(fg)) seq += ansiMap.at(fg) + ";";
    if (!bg.empty() && ansiMap.count("bg_" + bg)) seq += ansiMap.at("bg_" + bg) + ";";

    if (!seq.empty()) {
        if (seq.back() == ';') seq.pop_back();
        return ansi(seq) + text + ansi(ansiMap.at("reset"));
    }

    return text;
}

int RoundUI::consoleWidth() const {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return 80;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
        int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        return (width > 0) ? width : 80;
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
    if (hOut == INVALID_HANDLE_VALUE) return;

    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;

    int termWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    if (termWidth <= 0) return;

    SHORT targetCol = static_cast<SHORT>(termWidth / 2);

    COORD pos = csbi.dwCursorPosition;
    pos.X = targetCol;
    SetConsoleCursorPosition(hOut, pos);
#endif
}

std::string RoundUI::centerText(const std::string& text, int width) const {
    if (width <= 0) return text;

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
    if (padding < 0) padding = 0;

    int width = consoleWidth();
    if (width < 20) width = 80;

    int maxContentWidth = width - 4;
    if (maxContentWidth < 20) maxContentWidth = 20;

    auto truncate = [this, maxContentWidth](const std::string& s) {
        if (static_cast<int>(s.size()) <= maxContentWidth) return s;
        if (maxContentWidth <= 3) return s.substr(0, maxContentWidth);
        return s.substr(0, maxContentWidth - 3) + "...";
    };

    std::vector<std::string> content;
    content.reserve(lines.size());
    for (const auto& l : lines) content.push_back(truncate(l));

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
        return centerText(l, termWidth);
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

void RoundUI::print(const std::string &text) const {
    std::cout << centerText(text, consoleWidth()) << std::endl;
}

int RoundUI::askChoice(const std::string &prompt, const std::vector<std::string>& options, bool clearScreen) const{
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

        moveCursorToCenterRow();
        std::cout << "> ";
        if (!std::getline(std::cin, answer)) {
            std::cout << "\nInput closed\n";
            return -1;
        }

        answer = trim(answer);
        if (answer.empty()) continue;

        if (!safeStringToInt(answer, choice, 1, static_cast<int>(options.size()))) {
            std::cout << centerText("Invalid choice", consoleWidth()) << std::endl;
            pause(1000);
            continue;
        }

        return choice - 1;
    }
}

std::string RoundUI::ask(const std::string &prompt) const{
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

        moveCursorToCenterRow();
        std::cout << "> ";
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

        if (c < 0x80) {
            width += 1;
            ++i;
            continue;
        }

        uint32_t cp = 0;
        std::size_t len = 0;

        if ((c & 0xE0) == 0xC0 && i + 1 < s.size()) {
            cp = (c & 0x1F) << 6 |
                 (static_cast<unsigned char>(s[i+1]) & 0x3F);
            len = 2;
        } else if ((c & 0xF0) == 0xE0 && i + 2 < s.size()) {
            cp = (c & 0x0F) << 12 |
                 (static_cast<unsigned char>(s[i+1]) & 0x3F) << 6 |
                 (static_cast<unsigned char>(s[i+2]) & 0x3F);
            len = 3;
        } else if ((c & 0xF8) == 0xF0 && i + 3 < s.size()) {
            cp = (c & 0x07) << 18 |
                 (static_cast<unsigned char>(s[i+1]) & 0x3F) << 12 |
                 (static_cast<unsigned char>(s[i+2]) & 0x3F) << 6 |
                 (static_cast<unsigned char>(s[i+3]) & 0x3F);
            len = 4;
        } else {
            ++i;
            continue;
        }

        if (cp == 0xFE0F || cp == 0x200D) {
            i += len;
            continue;
        }

        if ( (cp >= 0x1F300 && cp <= 0x1FAFF) ||
             (cp >= 0x2600 && cp <= 0x26FF) ||
             (cp >= 0x2700 && cp <= 0x27BF) ||
             cp == 0x2B50
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
    if (symbols.empty()) {
        std::cerr << "Error: Symbols vector is empty\n";
        return;
    }

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
    int titleLen = static_cast<int>(title.size());

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

void RoundUI::renderWheel(const std::vector<RouletteTile>& wheel, const int& spunTile) {
    if (wheel.empty()) {
        std::cerr << "Error: Wheel vector is empty\n";
        return;
    }

    if (spunTile < 0 || spunTile >= static_cast<int>(wheel.size())) {
        std::cerr << "Error: Invalid spunTile index\n";
        return;
    }

    enableAnsiColors();
    clear();

    int termWidth = consoleWidth();
    if (termWidth < 40) termWidth = 80;

    int n = static_cast<int>(wheel.size());

    std::string row;

    for (int offset = -4; offset <= 4; ++offset) {
        int idx = (spunTile + offset + n) % n;
        const auto& tile = wheel[idx];
        bool isCenter = (offset == 0);

        std::string num = std::to_string(tile.number);
        if (num.size() == 1) num = " " + num;

        std::string colored;
        if (tile.color == RouletteTileType::RED) {
            colored = colorize(" " + num + " ", "white", "red", isCenter);
        } else if (tile.color == RouletteTileType::BLACK) {
            colored = colorize(" " + num + " ", "white", "black", isCenter);
        } else {
            colored = colorize(" " + num + " ", "black", "green", isCenter);
        }

        if (isCenter) {
            colored = ">" + colored + "<";
        }

        if (!row.empty()) row += " ";
        row += colored;
    }

    std::string pointer = "▼";
    std::string rowPlain = stripAnsi(row);

    int innerWidth = std::max(
        displayWidthUtf8(rowPlain),
        displayWidthUtf8(pointer)
    );

    int boxWidth = innerWidth + 4;
    int leftPad  = std::max(0, (termWidth - boxWidth) / 2);
    std::string indent(leftPad, ' ');

    std::string top    = "+" + std::string(innerWidth + 2, '-') + "+";
    std::string bottom = top;

    // pointer line
    int ptrW = displayWidthUtf8(pointer);
    int ptrSpaces = std::max(0, innerWidth - ptrW);
    int ptrLeft  = ptrSpaces / 2;
    int ptrRight = ptrSpaces - ptrLeft;

    std::string pointerLine =
        "| " + std::string(ptrLeft, ' ') + pointer +
        std::string(ptrRight, ' ') + " |";

    int rowW = displayWidthUtf8(rowPlain);
    int rowSpaces = std::max(0, innerWidth - rowW);
    int rowLeft  = rowSpaces / 2;
    int rowRight = rowSpaces - rowLeft;

    std::string rowLine =
        "| " + std::string(rowLeft, ' ') + row +
        std::string(rowRight, ' ') + " |";

    std::string title = " ROULETTE WHEEL ";
    int titleW = displayWidthUtf8(title);
    int titleSpaces = std::max(0, innerWidth - titleW);
    int titleLeft = titleSpaces / 2;
    int titleRight = titleSpaces - titleLeft;

    std::string titleLine =
        "| " + std::string(titleLeft, ' ') + title +
        std::string(titleRight, ' ') + " |";

    std::cout << indent << top        << "\n";
    std::cout << indent << titleLine  << "\n";
    std::cout << indent << top        << "\n";
    std::cout << indent << pointerLine<< "\n";
    std::cout << indent << rowLine    << "\n";
    std::cout << indent << bottom     << "\n";
}

std::string RoundUI::centerColored(const std::string& s, int termWidth) const {
    if (termWidth <= 0) return s;

    int realWidth = displayWidthUtf8(stripAnsi(s));
    if (realWidth >= termWidth) return s;

    int padding = (termWidth - realWidth) / 2;
    return std::string(padding, ' ') + s;
}

std::string RoundUI::stripAnsi(const std::string& s) const {
    std::string out;
    out.reserve(s.size());

    for (std::size_t i = 0; i < s.size(); ) {
        unsigned char c = static_cast<unsigned char>(s[i]);

        if (c == 0x1B && i + 1 < s.size() && s[i + 1] == '[') {
            i += 2;
            while (i < s.size() && s[i] != 'm') {
                ++i;
            }
            if (i < s.size()) ++i;
        } else {
            out.push_back(s[i]);
            ++i;
        }
    }

    return out;
}


void RoundUI::pause(const int ms) {
    if (ms <= 0) return;
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
        lines.emplace_back("");
        lines.emplace_back("No players yet!");
        lines.emplace_back("");
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

void RoundUI::waitForKey(const std::string& message) const {
    print(message);
    std::cin.get();
}

void RoundUI::waitForEnter(const std::string& message) const {
    print(message);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string RoundUI::askInput(const std::string &prompt,
                              const std::vector<std::string> &validInputs) const {
    if (prompt.empty()) {
        std::cerr << "Error: Prompt is empty\n";
        return "";
    }

    if (validInputs.empty()) {
        std::cerr << "Error: Valid inputs vector is empty\n";
        return "";
    }

    std::string answer;

    while (true) {
        clear();
        std::vector<std::string> lines = { prompt };
        drawBox("", lines);

        moveCursorToCenterRow();
        std::cout << "> ";
        if (!std::getline(std::cin, answer)) {
            std::cout << "\nInput closed\n";
            return "";
        }

        answer = trim(answer);
        if (std::find(validInputs.begin(), validInputs.end(), answer) != validInputs.end()) {
            return answer;
        }

        std::cout << centerText("Invalid input.", consoleWidth()) << std::endl;
        pause(1000);
    }
}

int RoundUI::askInput(const std::string &prompt, int min, int max) const {
    if (prompt.empty()) {
        std::cerr << "Error: Prompt is empty\n";
        return -1;
    }

    if (min > max) {
        std::cerr << "Error: min > max\n";
        return -1;
    }

    std::string answer;
    int value;

    while (true) {
        clear();
        std::vector<std::string> lines = {
            prompt,
            "Range: " + std::to_string(min) + " - " + std::to_string(max)
        };
        drawBox("", lines);

        moveCursorToCenterRow();
        std::cout << "> ";
        if (!std::getline(std::cin, answer)) {
            std::cout << "\nInput closed\n";
            return -1;
        }

        answer = trim(answer);
        if (safeStringToInt(answer, value, min, max)) {
            return value;
        }

        std::cout << centerText(
            "Input must be between " + std::to_string(min) +
            " and " + std::to_string(max),
            consoleWidth()) << std::endl;
        pause(1000);
    }
}









