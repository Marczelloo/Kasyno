//
// Created by moskw on 12.11.2025.
//

#include "SlotsGame.h"

SlotsGame::~SlotsGame() = default;

int SlotsGame::askForBet(int maxBalance) {
    ui.clear();
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
            return askForBet(maxBalance);
    }

    if (newBet > maxBalance) {
        ui.print("Insufficient balance for this bet (" + std::to_string(newBet) + "$)!");
        ui.print("Please choose a lower amount.");
        ui.waitForEnter();
        return askForBet(maxBalance);
    }

    if (newBet > maxBalance) {
        ui.print("Invalid choice, defaulting to custom amount!");
        newBet = 10;
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
    info.push_back(player.getName() + "'s Balance: " + std::to_string(player.getBalance()));
    info.push_back("Current bet: " + std::to_string(player.getCurrentBet()));
    if (lastScore >= 0) {
        if (lastScore > 0) {
            info.push_back("You won " + std::to_string(lastScore) + "!");
        } else {
            info.push_back("No win this time. Better luck next spin!");
        }
    }
    ui.drawBox("", info);

    int option = ui.askChoice("What would you like to do?",
                              TextRes::SLOTS_GAME_OPTIONS,
                              false);
    return option;
}

std::array<int, 3> SlotsGame::spinSlots() {
    auto getSymbol = [this]() {
        int chance = random->randInt(1, 100);
        return (chance <= 40) ? 0 : (chance <= 70) ? 1 : (chance <= 85) ? 2 :
               (chance <= 95) ? 3 : (chance <= 99) ? 4 : 5;
    };

    return {getSymbol(), getSymbol(), getSymbol()};
}

int SlotsGame::calculateScore(const std::array<int, 3>& slots,const int bet) {
    if (slots[0] == slots[1] && slots[1] == slots[2]) {
        return bet * tripletPayouts[slots[0]];
    }

    if (slots[0] == slots[1] || slots[1] == slots[2] || slots[0] == slots[2]) {
        int pairSymbol = (slots[0] == slots[1]) ? slots[0] :
                         (slots[1] == slots[2]) ? slots[1] : slots[0];
        return bet * pairPayouts[pairSymbol];
    }

    return 0;
}

GameState SlotsGame::playRound(Player &player) {
    slots = {-1, -1, -1};
    int bet = askForBet(player.getBalance());

    if (bet <= 0) {
        ui.print("Cannot continue playing. Returning to Game Menu.");
        ui.waitForEnter();
        return GameState::GAME_MENU;
    }

    player.setCurrentBet(bet);

    GameState newState = GameState::GAME_MENU;
    exit = false;

    while (!exit) {
        int option = renderInterface(player);

        switch (static_cast<SlotsOptions>(option)) {
            case SlotsOptions::SPIN: {
                if (player.getBalance() < bet) {
                    errorMessage = "Insufficient balance! Change bet or exit.";
                    break;
                }

                player.setCurrentBet(bet);
                player.updateBalance(-bet);

                std::array<int, 3> finalSlots = spinSlots();

                animateSpin(player, finalSlots);

                lastScore = calculateScore(slots, bet);

                if (lastScore > 0) {
                    player.updateBalance(lastScore);
                    int netProfit = lastScore - bet;
                    player.setWinnings(player.getWinnings() + netProfit);
                }

                break;
            }
            case SlotsOptions::CHANGE_BET: {
                int newBet = askForBet(player.getBalance());
                if (newBet <= 0) {
                    errorMessage = "Invalid bet amount. Keeping previous bet.";
                } else {
                    bet = newBet;
                    player.setCurrentBet(bet);
                }
                break;
            }
            case SlotsOptions::VIEW_PAYOUTS:
                displayPayouts();
                break;
            case SlotsOptions::EXIT_TO_GAME_MENU:
            case SlotsOptions::EXIT: {
                LeaderboardEntry entry{player.getName(), player.getBalance()};
                FileHandler::addEntry(entry);

                exit = true;

                newState = (option == static_cast<int>(SlotsOptions::EXIT_TO_GAME_MENU)) ?
                             GameState::GAME_MENU : GameState::EXIT;
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
    ui.clear();

    std::vector<std::string> payoutInfo;
    payoutInfo.push_back("=== PAYOUTS TABLE ===");
    payoutInfo.push_back("");
    payoutInfo.push_back("THREE OF A KIND:");

    for (size_t i = 0; i < TextRes::SLOT_SYMBOLS.size(); ++i) {
        std::string symbol = TextRes::SLOT_SYMBOLS[i];
        std::string line = symbol + " " + symbol + " " + symbol +
                          "  ->  x" + std::to_string(tripletPayouts[i]);
        payoutInfo.push_back(line);
    }

    payoutInfo.push_back("");
    payoutInfo.push_back("TWO OF A KIND:");

    for (size_t i = 0; i < TextRes::SLOT_SYMBOLS.size(); ++i) {
        std::string symbol = TextRes::SLOT_SYMBOLS[i];
        std::string line = symbol + " " + symbol + " ?  ->  x" +
                          std::to_string(pairPayouts[i]);
        payoutInfo.push_back(line);
    }

    ui.drawBox("", payoutInfo);
    ui.waitForEnter("Press ENTER to return");
}

void SlotsGame::animateSpin(Player& player, const std::array<int, 3>& finalSlots) {
    std::array<int, 3> spinCounts = {
        random->randInt(10, 20),
        random->randInt(15, 25),
        random->randInt(20, 30)
    };

    int maxSpins = std::max(spinCounts[0], std::max(spinCounts[1], spinCounts[2]));

    for (int spin = 0; spin < maxSpins; ++spin) {
        std::array<int, 3> currentSlots = slots;

        for (int i = 0; i < 3; ++i) {
            if (spin < spinCounts[i]) {
                currentSlots[i] = random->randInt(0, (int)TextRes::SLOT_SYMBOLS.size() - 1);
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
        info.push_back(player.getName() + "'s Balance: " + std::to_string(player.getBalance()));
        info.push_back("Current bet: " + std::to_string(player.getCurrentBet()));
        info.push_back("SPINNING...");
        ui.drawBox("", info);

        int delay = 50 + (spin * 10); // ms
        RoundUI::pause(delay);
    }

    slots = finalSlots;
}