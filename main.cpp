/**
 * @file main.cpp
 * @brief Entry point for the Casino Game Simulator application
 * @author Marczelloo
 * @date 2025-12-11
 */

#include <iostream>

#include "Casino.h"
#include "windows.h"
#include <io.h>
#include <fcntl.h>

/**
 * @brief Sets up console for UTF-8 encoding and ANSI color support
 *
 * This function configures the Windows console to:
 * - Enable Virtual Terminal Processing for ANSI escape codes
 * - Set input and output code page to UTF-8 for proper character display
 */
bool setupConsole() {
    // Get console output handle
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Failed to get console handle\n";
        return false;
    }

    // Enable ANSI escape codes support
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        std::cerr << "Error: Failed to get console mode\n";
        return false;
    }

    if (!SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        std::cerr << "Warning: Failed to enable ANSI support\n";
        // Continue execution - not critical
    }

    // Set UTF-8 encoding
    if (!SetConsoleOutputCP(CP_UTF8) || !SetConsoleCP(CP_UTF8)) {
        std::cerr << "Warning: Failed to set UTF-8 encoding\n";
        // Continue execution - not critical
    }

    return true;
}

/**
 * @brief Main entry point of the application
 *
 * Initializes the console settings and starts the casino application.
 *
 * @return int Exit code (0 for success)
 */
int main() {
    try {
        if (!setupConsole()) {
            std::cerr << "Warning: Console setup incomplete, display may be affected" << std::endl;
        }

        Casino casino = Casino();
        casino.run();

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Fatal error: unknown exception" << std::endl;
        return 1;
    }

}