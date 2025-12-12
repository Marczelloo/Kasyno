//
// Created by moskw on 09.10.2025.
//

#include "Casino.h"

#include "Resources/Enums.h"
#include "FileHandler.h"
#include "Games/BlackjackGame.h"
#include "Games/RouletteGame.h"
#include "Games/SlotsGame.h"
#include "Resources/TextRes.h"
#include "ExitHelper.h"

Casino::Casino()
    : player(nullptr),
      game(nullptr),
      ui(),
      random(),
      state(GameState::MAIN_MENU) {}

bool Casino::validatePlayer() {
    if (!player) {
        ui.print("Error: No active player! Returning to main menu.");
        ui.waitForEnter();
        state = GameState::MAIN_MENU;
        return false;
    }

    return true;
}

void Casino::run() {
    state = GameState::MAIN_MENU;

    while (state != GameState::EXIT) {
        try {
            switch (state) {
                case GameState::MAIN_MENU:
                    state = handleMainMenu();
                    break;
                case GameState::CASINO_MENU:
                    state = handleCasinoMenu();
                    break;
                case GameState::GAME_MENU:
                    state = handleGameMenu();
                    break;
                default:
                    ui.print("Invalid State, Quiting!");
                    state = GameState::EXIT;
                    break;
            }
        } catch (const std::bad_alloc& e) {
            ui.print("Error: Memory allocation failed!");
            ui.print(std::string("Details: ") + e.what());
            ui.waitForEnter();
            state = GameState::EXIT;
        } catch (const std::exception& e) {
            ui.print("Unexpected error occurred!");
            ui.print(std::string("Details: ") + e.what());
            ui.waitForEnter();
            state = GameState::MAIN_MENU;
        }
    }
}

GameState Casino::handleMainMenu() {
    RoundUI::clear();
    int option = ui.askChoice(TextRes::MAIN_MENU_TITLE, TextRes::MAIN_MENU_OPTIONS);

    switch (static_cast<MainMenuOptions>(option)) {
        case MainMenuOptions::MENU_CREATE_PLAYER: {
            auto res = createPlayer();
            if (res == CreatePlayerResult::Ok) return GameState::CASINO_MENU;
            if (res == CreatePlayerResult::Retry) return GameState::MAIN_MENU;
            return GameState::EXIT;
        }
        case MainMenuOptions::MENU_CHECK_LEADERBOARD:
            checkLeaderboard();
            return GameState::MAIN_MENU;
        case MainMenuOptions::MENU_EXIT:
            exitCasino();
            return state;
        default:
            ui.print("Invalid choice");
            return GameState::MAIN_MENU;
    }
}

CreatePlayerResult Casino::createPlayer() {
    const std::string name = ui.ask("Enter your name: ");

    if (name.empty()) {
        std::vector<std::string> errorInfo;
        errorInfo.emplace_back("Name cannot be empty!");
        ui.drawBox("ERROR", errorInfo);
        ui.waitForEnter();
        return CreatePlayerResult::Retry;
    }

    if (FileHandler::playerExists(name)) {
        std::vector<std::string> errorInfo;
        errorInfo.emplace_back("Player '" + name + "' already exists!");
        errorInfo.emplace_back("");
        errorInfo.emplace_back("Please choose a different name.");
        ui.drawBox("ERROR", errorInfo);
        ui.waitForEnter();
        return CreatePlayerResult::Retry;
    }

    const int minBalance = 5000;
    const int maxBalance = 10000;

    const int balance = random.randInt(minBalance, maxBalance);

    try {
        player = std::make_unique<Player>(name, balance);

        if (player->getName().empty() || player->getBalance() < 0) {
            ui.print("Error: Invalid player data");
            player.reset();
            return CreatePlayerResult::Retry;
        }

        std::vector<std::string> successInfo;
        successInfo.emplace_back("Player created successfully!");
        successInfo.emplace_back("");
        successInfo.emplace_back("Name: " + player->getName());
        successInfo.emplace_back("Balance: " + std::to_string(player->getBalance()) + "$");

        ui.drawBox("SUCCESS", successInfo);
        ui.waitForEnter();

        return CreatePlayerResult::Ok;
    } catch (const std::bad_alloc& e) {
        ui.print("Error: Memory allocation failed!");
        ui.print(std::string("Details: ") + e.what());
        ui.waitForEnter();
        return CreatePlayerResult::Exit;
    } catch (const std::exception& e) {
        ui.print("Error: Failed to create player!");
        ui.print(std::string("Details: ") + e.what());
        ui.waitForEnter();
        return CreatePlayerResult::Retry;
    }
}

void Casino::checkLeaderboard() {
    RoundUI::clear();

    try {
        auto entries = FileHandler::loadLeaderboard("leaderboard.txt");
        ui.leaderboard(TextRes::LEADERBOARD_TITLE, entries);
    } catch (const std::exception& e) {
        ui.print("Error loading leaderboard!");
        ui.print(std::string("Details: ") + e.what());
    }

    ui.waitForEnter("Press ENTER to return");
}

void Casino::exitCasino() {
    if (state == GameState::EXIT) return;

    if (!player) {
        state = GameState::EXIT;
        return;
    }

    try {
        if (confirmExitAndSave(ui, *player)) {
            state = GameState::EXIT;
        }
    } catch (const std::exception& e) {
        ui.print("Error saving player data!");
        ui.print(std::string("Details: ") + e.what());
        ui.waitForEnter();
        state = GameState::EXIT;
    }
}

GameState Casino::handleCasinoMenu() {
    if (!validatePlayer()) {
        return GameState::MAIN_MENU;
    }

    RoundUI::clear();
    int option = ui.askChoice(TextRes::CASINO_TITLE, TextRes::CASINO_OPTIONS);

    switch (static_cast<CasinoOptions>(option)) {
        case CasinoOptions::CASINO_SELECT_GAME:
            return GameState::GAME_MENU;
        case CasinoOptions::CASINO_CHECK_LEADERBOARD:
            checkLeaderboard();
            return GameState::CASINO_MENU;
        case CasinoOptions::CASINO_RETURN_TO_MAIN_MENU:
            return GameState::MAIN_MENU;
        case CasinoOptions::CASINO_EXIT:
            exitCasino();
            return state;
        default:
            ui.print("Invalid choice");
            return GameState::CASINO_MENU;
    }
}

GameState Casino::handleGameMenu() {
    if (!validatePlayer()) {
        return GameState::MAIN_MENU;
    }

    if (player->getBalance() <= 0) {
        std::vector<std::string> info;
        info.emplace_back("You have no money left!");
        info.emplace_back("");
        info.emplace_back("Your session has ended.");
        info.emplace_back("Final stats:");
        info.emplace_back("  Total Winnings: " + std::to_string(player->getWinnings()) + "$");

        ui.drawBox("GAME OVER", info);
        ui.waitForEnter();

        try {
            LeaderboardEntry entry{player->getName(), player->getBalance()};
            FileHandler::addEntry(entry);
        } catch (const std::exception& e) {
            ui.print("Error saving to leaderboard: " + std::string(e.what()));
        }

        return GameState::MAIN_MENU;
    }

    RoundUI::clear();
    int option = ui.askChoice(TextRes::GAME_SELECT_TITLE, TextRes::GAME_SELECT_OPTIONS);

    try {
        switch (static_cast<GameMenuOptions>(option)) {
            case GameMenuOptions::GAME_PLAY_SLOTS:
                game = std::make_unique<SlotsGame>(random);
                return game->playRound(*player);
            case GameMenuOptions::GAME_PLAY_ROULETTE:
                game = std::make_unique<RouletteGame>(random);
                return game->playRound(*player);
            case GameMenuOptions::GAME_PLAY_BLACKJACK:
                game = std::make_unique<BlackjackGame>(random);
                return game->playRound(*player);
            case GameMenuOptions::GAME_RETURN_TO_CASINO_MENU:
                game.reset();
                return GameState::CASINO_MENU;
            case GameMenuOptions::GAME_EXIT:
                exitCasino();
                return state;
            default:
                ui.print("Invalid choice");
                return GameState::CASINO_MENU;
        }
    } catch (const std::bad_alloc& e) {
        ui.print("Error: Failed to create game (memory allocation)!");
        ui.print(std::string("Details: ") + e.what());
        ui.waitForEnter();
        return GameState::GAME_MENU;
    } catch (const std::exception& e) {
        ui.print("Error: Failed to start game!");
        ui.print(std::string("Details: ") + e.what());
        ui.waitForEnter();
        return GameState::GAME_MENU;
    }
}

