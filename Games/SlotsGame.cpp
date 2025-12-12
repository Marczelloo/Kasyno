//
// Created by moskw on 12.11.2025.
//

#include "SlotsGame.h"

#include <stdexcept>

#include "../ExitHelper.h"

SlotsGame::SlotsGame(Rng &rng): Game("Slots", rng) {};

SlotsGame::~SlotsGame() = default;

int SlotsGame::askForBet(Player& player) {
    RoundUI::clear();

    int maxBalance = player.getBalance();

    ui.print("Your current balance is: " + std::to_string(maxBalance));

    if (maxBalance < 10) {
        ui.print("Insufficient balance! Minimum bet is 10$.");
        ui.print("Returning to Game Menu...");
        ui.waitForEnter();
        return -1;
    }

    int choice = ui.askChoice(TextRes::SLOTS_BET_OPTIONS_TITLE, TextRes::SLOTS_BET_OPTIONS);

    int newBet = 0;

    switch (static_cast<SlotsBetOptions>(choice)) {
        case SlotsBetOptions::BET_10:
            newBet = 10;
            break;
        case SlotsBetOptions::BET_20:
            newBet = 20;
            break;
        case SlotsBetOptions::BET_50:
            newBet = 50;
            break;
        case SlotsBetOptions::BET_100:
            newBet = 100;
            break;
        case SlotsBetOptions::BET_200:
            newBet = 200;
            break;
        case SlotsBetOptions::BET_500:
            newBet = 500;
            break;
        default:
            ui.print("Invalid choice! Please select a valid bet amount.");
            return askForBet(player);
    }

    if (!player.canAffordBet(newBet)) {
        ui.print("Insufficent balance for this bet (" + std::to_string(newBet) + "$)!");
        ui.print("Please choose a lower amount.");
        ui.waitForEnter();
        return askForBet(player);
    }

    return newBet;
}

int SlotsGame::renderInterface(const Player& player) {
    RoundUI::clear();

    std::vector<std::string> slotSymbols;
    if (slots[0] == -1) {
        slotSymbols = { "?", "?", "?" };
    } else {
        slotSymbols = {
            TextRes::SLOT_SYMBOLS[slots[0]],
            TextRes::SLOT_SYMBOLS[slots[1]],
            TextRes::SLOT_SYMBOLS[slots[2]]
        };
    }

    ui.renderSlots(slotSymbols);

    std::vector<std::string> info;
    info.emplace_back(player.getName() + "'s Balance: " + std::to_string(player.getBalance()));

    if (player.hasActiveBet()) {
        info.emplace_back("Current bet: " + std::to_string(player.getCurrentBet()));
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
                              TextRes::SLOTS_GAME_OPTIONS,
                              false);
    return option;
}

std::array<int, 3> SlotsGame::spinSlots() {
    auto getSymbol = [this]() {
        int chance = random.randInt(1, 100);
        return (chance <= 40) ? 0 : (chance <= 70) ? 1 : (chance <= 85) ? 2 :
               (chance <= 95) ? 3 : (chance <= 99) ? 4 : 5;
    };

    return {getSymbol(), getSymbol(), getSymbol()};
}

double SlotsGame::calculateMultiplier(const std::array<int, 3>& slots) {
    if (slots[0] == slots[1] && slots[1] == slots[2]) {
        return static_cast<double>(tripletPayouts[slots[0]]);
    }

    if (slots[0] == slots[1] || slots[1] == slots[2] || slots[0] == slots[2]) {
        int pairSymbol = (slots[0] == slots[1]) ? slots[0] :
                         (slots[1] == slots[2]) ? slots[1] : slots[0];
        return static_cast<double>(pairPayouts[pairSymbol]);
    }

    return 0.0;
}

GameState SlotsGame::playRound(Player &player) {
    slots = {-1, -1, -1};
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

        switch (static_cast<SlotsOptions>(option)) {
            case SlotsOptions::SPIN: {
                try {
                    if (!player.hasActiveBet()) {
                        player.placeBet(selectedBet);
                    }

                    std::array<int, 3> finalSlots = spinSlots();
                    animateSpin(player, finalSlots);

                    double multiplier = calculateMultiplier(finalSlots);

                    if (multiplier > 0.0) {
                        player.winBet(multiplier);
                        lastScore = static_cast<int>(selectedBet * multiplier);
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

                break;
            }
            case SlotsOptions::CHANGE_BET: {
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
            case SlotsOptions::VIEW_PAYOUTS:
                displayPayouts();
                break;
            case SlotsOptions::EXIT_TO_GAME_MENU: {
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
            case SlotsOptions::EXIT: {
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

void SlotsGame::displayPayouts() const {
    RoundUI::clear();

    std::vector<std::string> payoutInfo;
    payoutInfo.emplace_back("");
    payoutInfo.emplace_back("THREE OF A KIND:");

    for (size_t i = 0; i < TextRes::SLOT_SYMBOLS.size(); ++i) {
        std::string symbol = TextRes::SLOT_SYMBOLS[i];
        std::string line = symbol + " " + symbol + " " + symbol +
                          "  ->  x" + std::to_string(tripletPayouts[i]);
        payoutInfo.emplace_back(line);
    }

    payoutInfo.emplace_back("");
    payoutInfo.emplace_back("TWO OF A KIND:");

    for (size_t i = 0; i < TextRes::SLOT_SYMBOLS.size(); ++i) {
        std::string symbol = TextRes::SLOT_SYMBOLS[i];
        std::string line = symbol + " " + symbol + " ?  ->  x" +
                          std::to_string(pairPayouts[i]);
        payoutInfo.emplace_back(line);
    }

    ui.drawBox("PAYOUTS TABLE", payoutInfo);
    ui.waitForEnter("Press ENTER to return");
}

void SlotsGame::animateSpin(const Player& player, const std::array<int, 3>& finalSlots) {
    std::array<int, 3> spinCounts = {
        random.randInt(10, 20),
        random.randInt(15, 25),
        random.randInt(20, 30)
    };

    int maxSpins = std::max(spinCounts[0], std::max(spinCounts[1], spinCounts[2]));

    for (int spin = 0; spin < maxSpins; ++spin) {
        std::array<int, 3> currentSlots = slots;

        for (int i = 0; i < 3; ++i) {
            if (spin < spinCounts[i]) {
                currentSlots[i] = random.randInt(0, (int)TextRes::SLOT_SYMBOLS.size() - 1);
            } else {
                currentSlots[i] = finalSlots[i];
            }
        }

        RoundUI::clear();
        std::vector<std::string> displaySymbols = {
            TextRes::SLOT_SYMBOLS[currentSlots[0]],
            TextRes::SLOT_SYMBOLS[currentSlots[1]],
            TextRes::SLOT_SYMBOLS[currentSlots[2]]
        };
        ui.renderSlots(displaySymbols);

        std::vector<std::string> info;
        info.emplace_back(player.getName() + "'s Balance: " + std::to_string(player.getBalance()));
        info.emplace_back("Current bet: " + std::to_string(player.getCurrentBet()));
        info.emplace_back("SPINNING...");
        ui.drawBox("", info);

        int delay = 50 + (spin * 10); // ms
        RoundUI::pause(delay);
    }

    slots = finalSlots;
}