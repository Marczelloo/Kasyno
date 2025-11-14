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

Casino::Casino() {
    random = new Rng();
    game = nullptr;
    player = nullptr;
    state = GameState::MAIN_MENU;
}

Casino::~Casino() {
    if (player != nullptr) {
        delete player;
    }

    if (game != nullptr) {
        delete game;
    }

    if (random != nullptr) {
        delete random;
    }
}

void Casino::run() {
    state = GameState::MAIN_MENU;

    while (state != GameState::EXIT) {
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
    }

    if (player != nullptr) {
        LeaderboardEntry entry{player->getName(), player->getBalance()};
        FileHandler::addEntry(entry);
    }
}

GameState Casino::handleMainMenu() {
    ui.clear();
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
            return GameState::EXIT;
        default:
            ui.print("Invalid choice");
            return GameState::MAIN_MENU;
    }
}

CreatePlayerResult Casino::createPlayer() {
    const std::string name = ui.ask("Enter your name: ");

    if (name.empty()) {
        std::vector<std::string> errorInfo;
        errorInfo.push_back("Name cannot be empty!");
        ui.drawBox("ERROR", errorInfo);
        ui.waitForEnter();
        return CreatePlayerResult::Retry;
    }

    if (FileHandler::playerExists(name)) {
        std::vector<std::string> errorInfo;
        errorInfo.push_back("Player '" + name + "' already exists!");
        errorInfo.push_back("");
        errorInfo.push_back("Please choose a different name.");
        ui.drawBox("ERROR", errorInfo);
        ui.waitForEnter();
        return CreatePlayerResult::Retry;
    }

    const int minBalance = 500;
    const int maxBalance = 2000;

    const int balance = random->randInt(minBalance, maxBalance);

    if (player != nullptr) {
        delete player;
    }

    player = new Player(name, balance);

    if (player->getName().empty() || player->getBalance() < 0) {
        ui.print("Error: Invalid player data");
        delete player;
        player = nullptr;
        return CreatePlayerResult::Retry;
    }

    std::vector<std::string> successInfo;
    successInfo.push_back("Player created successfully!");
    successInfo.push_back("");
    successInfo.push_back("Name: " + player->getName());
    successInfo.push_back("Balance: " + std::to_string(player->getBalance()));

    ui.drawBox("SUCCESS", successInfo);
    ui.waitForEnter();

    return CreatePlayerResult::Ok;
}

void Casino::checkLeaderboard() {
    ui.clear();
    auto entries = FileHandler::loadLeaderboard("leaderboard.txt");

    ui.leaderboard(TextRes::LEADERBOARD_TITLE, entries);

    ui.waitForEnter("Press ENTER to return");

    return;
}

void Casino::exitCasino() {
    if (state == GameState::EXIT) return;

    const int response = ui.askChoice("Are you sure you want to exit the casino?", {"Yes", "No"});

    if (response != 0) {
        ui.print("Exit cancelled.");
        return;
    }

    ui.print("Exiting...");

    if (player != nullptr) {
        LeaderboardEntry entry{player->getName(), player->getBalance()};
        FileHandler::addEntry(entry);
    }
}

GameState Casino::handleCasinoMenu() {
    ui.clear();
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
            return GameState::EXIT;
        default:
            ui.print("Invalid choice");
            return GameState::CASINO_MENU;
    }
}

GameState Casino::handleGameMenu() {
    ui.clear();
    int option = ui.askChoice(TextRes::GAME_SELECT_TITLE, TextRes::GAME_SELECT_OPTIONS);

    switch (static_cast<GameMenuOptions>(option)) {
        case GameMenuOptions::GAME_PLAY_SLOTS:
            if (game != nullptr) delete game;
            game = new SlotsGame(random);
            return game->playRound(*player);
        case GameMenuOptions::GAME_PLAY_ROULETTE:
            if (game != nullptr) delete game;
            game = new RouletteGame(random);
            return game->playRound(*player);
        case GameMenuOptions::GAME_PLAY_BLACKJACK:
            if (game != nullptr) delete game;
            game = new BlackjackGame(random);
            return game->playRound(*player);
        case GameMenuOptions::GAME_RETURN_TO_CASINO_MENU:
            return GameState::CASINO_MENU;
        case GameMenuOptions::GAME_EXIT:
            exitCasino();
            return GameState::EXIT;
        default:
            ui.print("Invalid choice");
            return GameState::CASINO_MENU;
    }
}

// GameState Casino::handleGamePlay() {
//     if (game == nullptr || player == nullptr) {
//         ui.print("Error: Game or player not initialized");
//         return GameState::EXIT;
//     }
//
//     if (player->getCurrentBet() <= 0 || changeBet) {
//         int newBet = handleBetMenu(player->getBalance());
//
//         if (newBet <= 0 || newBet > player->getBalance()) {
//             ui.print("Error: Invalid bet amount, returning to Casino Menu.");
//             return GameState::PLAYING;
//         }
//
//         player->setCurrentBet(newBet);
//         changeBet = false;
//     }
//
//     player->updateBalance(-player->getCurrentBet());
//
//     game->playRound(*player, player->getCurrentBet());
//
//     if (player->getBalance() <= 0) {
//         ui.print("You have run out of balance! Returning to Casino Menu.");
//         player->setCurrentBet(0);
//         return GameState::CASINO_MENU;
//     }
//
//     int option = ui.askChoice(TextRes::AFTER_GAME_TITLE, TextRes::AFTER_GAME_OPTIONS);
//
//     switch (static_cast<AfterGameOptions>(option)) {
//         case AfterGameOptions::AFTER_PLAY_ANOTHER_ROUND:
//             return GameState::PLAYING;
//         case AfterGameOptions::AFTER_CHANGE_BET_AND_PLAY_ANOTHER_ROUND:
//             changeBet = true;
//             return GameState::PLAYING;
//         case AfterGameOptions::AFTER_PLAY_ANOTHER_GAME:
//         case AfterGameOptions::AFTER_RETURN_TO_GAME_MENU:
//             player->setCurrentBet(0);
//             return GameState::GAME_MENU;
//         case AfterGameOptions::AFTER_RETURN_TO_CASINO_MENU:
//             player->setCurrentBet(0);
//             return GameState::CASINO_MENU;
//         case AfterGameOptions::AFTER_EXIT:
//             exitCasino();
//             return GameState::EXIT;
//         default:
//             ui.print("Invalid choice, returning to Casino Menu");
//             player->setCurrentBet(0);
//             return GameState::CASINO_MENU;
//     }
// }
// int Casino::handleBetMenu(int currentBalance) {
//     int betAmount = 0;
//
//     ui.print(TextRes::BET_SELECT_TITLE);
//     ui.print("Current balance: " + std::to_string(player->getBalance()));
//     int option = ui.askChoice(TextRes::BET_SELECT_TITLE, TextRes::BET_SELECT_OPTIONS);
//
//     switch (static_cast<BetOptions>(option)) {
//         case BetOptions::BET_ALL_IN:
//             betAmount = currentBalance;
//             break;
//         case BetOptions::BET_HALF:
//             betAmount = currentBalance / 2;
//             break;
//         case BetOptions::BET_QUARTER:
//             betAmount = currentBalance / 4;
//             break;
//         case BetOptions::BET_CUSTOM:
//             betAmount = ui.askInput("Please enter your bet amount: ", 1, currentBalance);
//             break;
//         default:
//             ui.print("Invalid choice, defaulting to custom amount!");
//             break;
//     }
//
//     if (betAmount <= 0) {
//         ui.print("Invalid bet amount, setting to 1");
//         betAmount = 1;
//     } else if (betAmount > currentBalance) {
//         ui.print("Bet amount exceeds current balance, setting to max balance");
//         betAmount = currentBalance;
//     }
//
//     ui.print("Your bet is set to: " + std::to_string(betAmount));
//
//     return betAmount;
// }

