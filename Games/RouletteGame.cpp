//
// Created by moskw on 12.11.2025.
//

#include "RouletteGame.h"
#include <array>
#include <algorithm>
#include <thread>
#include <chrono>

#include "../ExitHelper.h"

/// @brief Payout multipliers for each bet type
constexpr std::array<int, 8> ROULETTE_PAYOUT_MULTIPLIERS = {
    2,  // RED
    2,  // BLACK
    35, // GREEN
    35, // NUMBER
    2,  // ODD
    2,  // EVEN
    2,  // LOW
    2   // HIGH
};

RouletteGame::RouletteGame(Rng &rng): Game("Roulette", rng),
    lastScore(-1),
    betType(RouletteBetType::BET_RED),
    betNumber(-1),
    wheel(initWheel()),
    spunTile(-1) {};

RouletteGame::~RouletteGame() = default;

RouletteTileType RouletteGame::getColorForNumber(int number) const {
    if (number == 0) return RouletteTileType::GREEN;

    static const std::array<int, 18> redNumbers = {
        1, 3, 5, 7, 9, 12, 14, 16, 18,
        19, 21, 23, 25, 27, 30, 32, 34, 36
    };

    return (std::find(redNumbers.begin(), redNumbers.end(), number) != redNumbers.end())
               ? RouletteTileType::RED
               : RouletteTileType::BLACK;
}

std::vector<RouletteTile> RouletteGame::initWheel() {
    static const std::vector<int> numbers = {
        0, 32, 15, 19, 4, 21, 2, 25,
        17, 34, 6, 27, 13, 36, 11, 30,
        8, 23, 10, 5, 24, 16, 33, 1,
        20, 14, 31, 9, 22, 18, 29, 7,
        28, 12, 35, 3, 26
    };

    std::vector<RouletteTile> wheel;
    wheel.reserve(numbers.size());

    for (int n : numbers) {
        wheel.emplace_back(RouletteTile{
            getColorForNumber(n),
            n
        });
    }

    return wheel;
}

int RouletteGame::spinWheel() {
    return random.randInt(0, static_cast<int>(wheel.size()) - 1);
}

void RouletteGame::animateSpin(const Player& player, int resultIndex) {
    int n = static_cast<int>(wheel.size());
    if (n == 0) return;

    int startIndex = (spunTile >= 0 && spunTile < n)
                        ? spunTile
                        : random.randInt(0, n - 1);

    int extraRounds = random.randInt(2, 4);

    int delta = (resultIndex - startIndex + n) % n;
    int totalSteps = extraRounds * n + delta;

    float minDelayMs = 15.0f;
    float MaxDelaysMs = 180.0f;

    for (int step = 0; step <= totalSteps; ++step) {
        int currentIndex = (startIndex + step) % n;

        RoundUI::clear();

        ui.renderWheel(wheel, currentIndex);
        std::vector<std::string> info;
        info.emplace_back(player.getName() + "'s Balance: " + std::to_string(player.getBalance()));
        info.emplace_back("Current bet: " + std::to_string(player.getCurrentBet()) +
                         "$ - " + TextRes::ROULETTE_BET_TYPES[static_cast<int>(betType)] +
                         (betType == RouletteBetType::BET_NUMBER
                              ? " (" + std::to_string(betNumber) + ")"
                              : ""));
        info.emplace_back("");
        info.emplace_back("Spinning the wheel...");
        ui.drawBox("", info);

        float t = (totalSteps > 0)
                        ? static_cast<float>(step) / static_cast<float>(totalSteps)
                        : 1.0f;

        float delay = minDelayMs + (MaxDelaysMs - minDelayMs) * (t * t);

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay)));
    }

    spunTile = resultIndex;
}

int RouletteGame::askForBet(Player& player) {
    RoundUI::clear();

    int maxBalance = player.getBalance();

    ui.print("Your current balance is: " + std::to_string(maxBalance));

    if (maxBalance < 1) {
        ui.print("Insufficient balance! Minimum bet is 1$.");
        ui.print("Returning to Game Menu...");
        ui.waitForEnter();
        return -1;
    }

    int choice = ui.askChoice(TextRes::ROULETTE_BET_OPTIONS_TITLE, TextRes::ROULETTE_BET_TYPES);

    RouletteBetType newBetType = static_cast<RouletteBetType>(choice);

    if (newBetType == RouletteBetType::BET_NUMBER) {
        int input = ui.askInput("Enter the number you want to bet on (0-36): ",
                                0, 36);
        betNumber = input;
    }

    choice = ui.askChoice(TextRes::BET_SELECT_TITLE,
                          TextRes::BET_SELECT_OPTIONS);

    int newBetAmount = 0;

    switch (static_cast<BetOptions>(choice)) {
        case BetOptions::BET_ALL_IN:
            newBetAmount = maxBalance;
            break;
        case BetOptions::BET_HALF:
            newBetAmount = maxBalance / 2;
            break;
        case BetOptions::BET_QUARTER:
            newBetAmount = maxBalance / 4;
            break;
        case BetOptions::BET_CUSTOM: {
            newBetAmount = ui.askInput(
                "Enter your bet amount (1 - " + std::to_string(maxBalance) + "): ",
                1,
                maxBalance
            );
            break;
        }
        default:
            ui.print("Invalid choice! Please select a valid bet amount.");
            return askForBet(player);
    }

    if (newBetAmount <= 0 || !player.canAffordBet(newBetAmount)) {
        ui.print("Insufficient balance for this bet (" + std::to_string(newBetAmount) + "$)!");
        ui.waitForEnter();
        return askForBet(player);
    }

    betType = newBetType;
    return newBetAmount;
}

double RouletteGame::calculateMultiplier(int selectedTile) {
    if (selectedTile < 0 || selectedTile >= static_cast<int>(wheel.size())) {
        return 0.0;
    }

    const RouletteTile& tile = wheel[selectedTile];
    bool win = false;

    switch (betType) {
        case RouletteBetType::BET_RED:
            win = (tile.color == RouletteTileType::RED);
            break;
        case RouletteBetType::BET_BLACK:
            win = (tile.color == RouletteTileType::BLACK);
            break;
        case RouletteBetType::BET_GREEN:
            win = (tile.color == RouletteTileType::GREEN);
            break;
        case RouletteBetType::BET_NUMBER:
            win = (tile.number == betNumber);
            break;
        case RouletteBetType::BET_ODD:
            win = (tile.number != 0 && tile.number % 2 == 1);
            break;
        case RouletteBetType::BET_EVEN:
            win = (tile.number != 0 && tile.number % 2 == 0);
            break;
        case RouletteBetType::BET_LOW:
            win = (tile.number >= 1 && tile.number <= 18);
            break;
        case RouletteBetType::BET_HIGH:
            win = (tile.number >= 19 && tile.number <= 36);
            break;
        default:
            return 0.0;
    }

    if (!win) {
        return 0.0;
    }

    return static_cast<double>(
        ROULETTE_PAYOUT_MULTIPLIERS[static_cast<int>(betType)]
    );
}

void RouletteGame::displayPayouts() const {
    RoundUI::clear();

    std::vector<std::string> payoutInfo;
    payoutInfo.reserve(TextRes::ROULETTE_BET_TYPES.size() + 2);

    for (std::size_t i = 0; i < TextRes::ROULETTE_BET_TYPES.size(); ++i) {
        const std::string& type = TextRes::ROULETTE_BET_TYPES[i];
        int multiplier = ROULETTE_PAYOUT_MULTIPLIERS[i];

        std::string line = type + " ->  x" + std::to_string(multiplier);
        payoutInfo.emplace_back(std::move(line));
    }

    ui.drawBox("PAYOUTS TABLE", payoutInfo);
    ui.waitForEnter("Press ENTER to return");
}

int RouletteGame::renderInterface(const Player &player) {
    RoundUI::clear();

    ui.renderWheel(wheel, spunTile);

    std::vector<std::string> info;
    info.emplace_back(player.getName() + "'s Balance: " + std::to_string(player.getBalance()));
    if (player.hasActiveBet()) {
        info.emplace_back("Current bet: " + std::to_string(player.getCurrentBet()) +
                         "$ - " + TextRes::ROULETTE_BET_TYPES[static_cast<int>(betType)] +
                         (betType == RouletteBetType::BET_NUMBER
                              ? " (" + std::to_string(betNumber) + ")"
                              : ""));
    }

    if (lastScore >= 0) {
        if (lastScore > 0) {
            info.emplace_back("You won " + std::to_string(lastScore) + "!");
        } else {
            info.emplace_back("No win this time. Better luck next spin!");
        }
    }

    if (!errorMessage.empty()) {
        info.emplace_back("");
        info.emplace_back("Error: " + errorMessage);
        errorMessage.clear();
    }

    ui.drawBox("", info);

    int option = ui.askChoice("What would you like to do?",
                              TextRes::ROULETTE_GAME_OPTIONS,
                              false);

    return option;
}

GameState RouletteGame::playRound(Player &player) {
    spunTile = -1;
    lastScore = -1;

    int bet = askForBet(player);

    if (bet <= 0) {
        ui.print("Cannot continue playing. Returning to Game Menu.");
        ui.waitForEnter();
        return GameState::GAME_MENU;
    }

    int selectedBet = bet;
    GameState newState = GameState::GAME_MENU;
    exit = false;

    while (!exit) {
        int option = renderInterface(player);

        switch (static_cast<RouletteOptions>(option)) {
            case RouletteOptions::SPIN: {
                try {
                    if (!player.hasActiveBet()) {
                        player.placeBet(selectedBet);
                    }

                    int resultIndex = spinWheel();
                    animateSpin(player, resultIndex);

                    double multiplier = calculateMultiplier(resultIndex);

                    if (multiplier > 0.0) {
                        player.winBet(multiplier);
                        lastScore = static_cast<int>(selectedBet);
                    } else {
                        player.loseBet();
                        lastScore = 0;
                    }
                } catch (const std::invalid_argument& e) {
                    errorMessage = "Bet error: " + std::string(e.what());
                    lastScore = -1;
                } catch (const std::logic_error& e) {
                    errorMessage = "Logic error: " + std::string(e.what());
                    lastScore = -1;
                }


                if (player.getBalance() < bet) {
                    errorMessage = "Insufficient balance to place the bet.";
                    break;
                }

                break;
            }
            case RouletteOptions::CHANGE_BET: {
                if (player.hasActiveBet()) {
                    try {
                        player.cancelBet();
                    } catch (const std::exception& e) {
                        errorMessage = "Cancel error: " + std::string(e.what());
                        break;
                    }
                }

                int newBet = askForBet(player);

                if (newBet <= 0) {
                    errorMessage = "Bet selection cancelled!";
                } else {
                    selectedBet = newBet;
                    lastScore = -1;
                }
                break;
            }
            case RouletteOptions::VIEW_PAYOUTS: {
                displayPayouts();
                break;
            }
            case RouletteOptions::EXIT_TO_GAME_MENU: {
                if (player.hasActiveBet()) {
                    try {
                        player.cancelBet();
                    } catch (const std::exception& e) {
                        errorMessage = "Failed to cancel bet: " + std::string(e.what());
                        break;
                    }
                }

                exit = true;
                newState = GameState::GAME_MENU;
                break;
            }
            case RouletteOptions::EXIT: {
                if (confirmExitAndSave(ui, player)) {
                    exit = true;
                    newState = GameState::EXIT;
                } else {
                    errorMessage = "Exit cancelled.";
                }
                break;
            }
            default:
                errorMessage = "Invalid choice, please try again.";
                break;
        }
    }

    return newState;
}