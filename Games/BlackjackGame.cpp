//
// Created by moskw on 12.11.2025.
//

#include "BlackjackGame.h"
#include "../ExitHelper.h"

BlackjackGame::BlackjackGame(Rng &rng): Game("Blackjack", rng),
    lastScore(-1),
    deck(initializeDeck()) {};

BlackjackGame::~BlackjackGame() = default;

std::vector<std::vector<Card>> BlackjackGame::initializeDeck() {
    std::vector<std::vector<Card>> newDeck;

    for (int suitIdx = 0; suitIdx < 4; suitIdx++) {
        std::vector<Card> suitCards;
        for (int rankIdx = 0; rankIdx < 13; rankIdx++) {
            Card card;
            card.suit = static_cast<Suit>(suitIdx);

            switch (rankIdx) {
                case 0:
                    card.rank = "A";
                    card.value = 11;
                    break;
                case 10: case 11: case 12:
                    card.rank = (rankIdx == 10 ? "J" : (rankIdx == 11 ? "Q" : "K"));
                    card.value = 10;
                    break;
                default:
                    card.rank = std::to_string(rankIdx + 1);
                    card.value = rankIdx + 1; break;
            }
            suitCards.push_back(card);
        }
        newDeck.push_back(suitCards);
    }

    return newDeck;
}

std::vector<std::vector<Card>> BlackjackGame::shuffleDeck() {
    std::vector<Card> flatDeck;
    for (const auto& suitCards : deck) {
        flatDeck.insert(flatDeck.end(), suitCards.begin(), suitCards.end());
    }

    for (size_t i = flatDeck.size() - 1; i > 0; --i) {
        size_t j = static_cast<size_t>(random.randInt(0, static_cast<int>(i)));
        std::swap(flatDeck[i], flatDeck[j]);
    }

    std::vector<std::vector<Card>> shuffledDeck;
    size_t cardsPerSuit = 13;

    for (size_t i = 0; i < flatDeck.size(); i += cardsPerSuit) {
        std::vector<Card> suitCards(
            flatDeck.begin() + i,
            flatDeck.begin() + std::min(i + cardsPerSuit,
            flatDeck.size()));
        shuffledDeck.push_back(suitCards);
    }

    return shuffledDeck;
}

Card BlackjackGame::drawCard() {
    for (auto& suitCards : deck) {
        if (!suitCards.empty()) {
            Card drawnCard = suitCards.back();
            suitCards.pop_back();
            return drawnCard;
        }
    }

    deck = shuffleDeck();
    return drawCard();
}

int BlackjackGame::askForBet(Player& player) {
    RoundUI::clear();

    int maxBalance = player.getBalance();

    ui.print("Your current balance is: " + std::to_string(maxBalance));

    if (maxBalance < 1) {
        ui.print("Insufficient balance! Minimum bet is 1$.");
        ui.print("Returning to Game Menu...");
        ui.waitForEnter();
        return -1;
    }

    int choice  = ui.askChoice(TextRes::BET_SELECT_TITLE, TextRes::BET_SELECT_OPTIONS);

    int newBet = 0;

    switch (static_cast<BetOptions>(choice)) {
        case BetOptions::BET_ALL_IN:
            newBet = maxBalance;
            break;
        case BetOptions::BET_HALF:
            newBet = maxBalance / 2;
            break;
        case BetOptions::BET_QUARTER:
            newBet = maxBalance / 4;
            break;
        case BetOptions::BET_CUSTOM: {
            int bet = ui.askInput("Enter your bet amount (1 - " + std::to_string(maxBalance) + "): ", 1, maxBalance);
            newBet = (bet > 0 && bet <= maxBalance) ? bet : maxBalance;
            break;
        }
        default:
            ui.print("Invalid choice, defaulting to custom amount!");
            int bet = ui.askInput("Enter your bet amount (1 - " + std::to_string(maxBalance) + "): ", 1, maxBalance);
            newBet = (bet > 0 && bet <= maxBalance) ? bet : maxBalance;
    }

    if (newBet <= 0 || !player.canAffordBet(newBet)) {
        ui.print("Insufficient balance for this bet (" + std::to_string(newBet) + "$)!");
        ui.waitForEnter();
        return askForBet(player);
    }

    return newBet;
}

void BlackjackGame::displayPayouts() const {
    RoundUI::clear();

    std::vector<std::string> payoutInfo;
    payoutInfo.emplace_back("Blackjack (Ace + 10-value card): 3:2 payout");
    payoutInfo.emplace_back("Win: 1:1 payout");
    payoutInfo.emplace_back("Push: Bet returned");
    ui.drawBox("PAYOUTS TABLE", payoutInfo);
    ui.waitForEnter("Press ENTER to return");
}

void BlackjackGame::displayRules() const {
    RoundUI::clear();

    std::vector<std::string> rulesInfo;
    rulesInfo.emplace_back("1. Goal:");
    rulesInfo.emplace_back("   Get as close to 21 as possible without exceeding it.");
    rulesInfo.emplace_back("");
    rulesInfo.emplace_back("2. Card Values:");
    rulesInfo.emplace_back("   - Number cards = face value");
    rulesInfo.emplace_back("   - J, Q, K = 10");
    rulesInfo.emplace_back("   - Ace = 1 or 11, whichever is more beneficial");
    rulesInfo.emplace_back("");
    rulesInfo.emplace_back("3. Initial Deal:");
    rulesInfo.emplace_back("   - You receive 2 cards.");
    rulesInfo.emplace_back("   - The dealer receives 1 card (European Blackjack).");
    rulesInfo.emplace_back("");
    rulesInfo.emplace_back("4. Player Actions:");
    rulesInfo.emplace_back("   - HIT: Draw another card.");
    rulesInfo.emplace_back("   - STAND: End your turn.");
    rulesInfo.emplace_back("   - DOUBLE DOWN: Available only on your first two cards.");
    rulesInfo.emplace_back("       Doubles your bet, draws exactly one card, then you must stand.");
    rulesInfo.emplace_back("   - SPLIT: Available when your first two cards have the same value.");
    rulesInfo.emplace_back("       Splits them into two separate hands with separate bets.");
    rulesInfo.emplace_back("   - SURRENDER: Only on your first action.");
    rulesInfo.emplace_back("       You forfeit the hand and lose only half your bet.");
    rulesInfo.emplace_back("");
    rulesInfo.emplace_back("5. Split Hands:");
    rulesInfo.emplace_back("   - Each split hand is played independently.");
    rulesInfo.emplace_back("   - Each hand has its own bet.");
    rulesInfo.emplace_back("");
    rulesInfo.emplace_back("6. Dealer Rules:");
    rulesInfo.emplace_back("   - After all your hands are played, the dealer draws a second card.");
    rulesInfo.emplace_back("   - The dealer must hit until reaching at least 17.");
    rulesInfo.emplace_back("");
    rulesInfo.emplace_back("7. Winning:");
    rulesInfo.emplace_back("   - If your hand exceeds 21, you bust and lose that hand.");
    rulesInfo.emplace_back("   - If the dealer busts, all active player hands win.");
    rulesInfo.emplace_back("   - Otherwise, the closest value to 21 wins.");
    rulesInfo.emplace_back("   - Equal totals result in a 'push' (your bet is returned).");
    rulesInfo.emplace_back("");
    rulesInfo.emplace_back("8. Blackjack:");
    rulesInfo.emplace_back("   - If your first two cards total 21 (Ace + 10-value card),");
    rulesInfo.emplace_back("     you win 1.5x your bet unless the dealer also gets 21.");

    ui.drawBox("BLACKJACK RULES", rulesInfo);
    ui.waitForEnter("Press ENTER to return");
}

int BlackjackGame::renderInterface(const Player &player) {
    RoundUI::clear();

    std::vector<std::string> info;
    info.emplace_back(player.getName() + "'s Balance: " + std::to_string(player.getBalance()));

    if (player.hasActiveBet()) {
        info.emplace_back("Current bet: " + std::to_string(player.getCurrentBet()) + "$");
    }

    if (lastScore >= 0) {
        if (lastScore > 0) {
            info.emplace_back("");
            info.emplace_back("You won " + std::to_string(lastScore) + "!");
        } else {
            info.emplace_back("");
            info.emplace_back("No win this time. Better luck next round!");
        }
    }

    if (!errorMessage.empty()) {
        info.emplace_back("");
        info.emplace_back(errorMessage);
        errorMessage.clear();
    }

    ui.drawBox("BLACKJACK", info);

    int option = ui.askChoice("What would you like to do?",
                                  TextRes::BLACKJACK_GAME_OPTIONS,
                                  false);

    return option;
}

void BlackjackGame::renderRound(const Player &player, bool playerTurn, const std::string &winningInfo) const {
    RoundUI::clear();

    std::vector<std::string> roundInfo;

    std::string dealersHandStr = "Dealer's Hand: ";
    int dealersSum = 0;
    for (const auto& card : dealerHand) {
        dealersHandStr += card.rank + " ";
        dealersSum += card.value;
    }

    dealersHandStr += " (" + std::to_string(dealersSum) + ")";

    roundInfo.emplace_back(dealersHandStr);
    roundInfo.emplace_back("");
    roundInfo.emplace_back("");
    roundInfo.emplace_back("");

    size_t handIndex = 1;
    for (const auto& hand : playerHand) {
        int handSum = 0;
        std::string handStr = "Hand " + std::to_string(handIndex) + ": ";

        for (const auto& card : hand) {
            handStr += card.rank + " ";
            handSum += card.value;
        }

        handStr += " (" + std::to_string(handSum) + ")";
        roundInfo.emplace_back(handStr);
        handIndex++;
    }

    roundInfo.emplace_back("");
    roundInfo.emplace_back(player.getName() + "'s Turn");

    std::string turnInfo = playerTurn ? "Your Turn" : "Dealer's Turn";
    ui.drawBox("BLACKJACK ROUND - " + turnInfo, roundInfo);

    roundInfo.clear();

    roundInfo.emplace_back(player.getName() + "'s Balance: " + std::to_string(player.getBalance()));
    roundInfo.emplace_back("Current bet: " + std::to_string(player.getCurrentBet()));
    roundInfo.emplace_back("");
    roundInfo.emplace_back(winningInfo);

    ui.drawBox("", roundInfo);

    ui.waitForEnter();
}

double BlackjackGame::handleRound(Player &player) {
    deck = shuffleDeck();

    playerHand.clear();
    handBets.clear();
    dealerHand.clear();
    surrendered.clear();

    playerHand.emplace_back();
    auto &hand0 = playerHand[0];

    hand0.push_back(drawCard());
    hand0.push_back(drawCard());
    dealerHand.push_back(drawCard());

    handBets.push_back(player.getCurrentBet());
    surrendered.push_back(false);

    std::string roundInfo = "Starting round.";
    renderRound(player, true, roundInfo);

    int playerSum = hand0[0].value + hand0[1].value;

    if (playerSum == 21) {
        roundInfo = "Blackjack! You win 2.5 times your bet.";
        lastScore = static_cast<int>(player.getCurrentBet() * 2.5);
        renderRound(player, false, roundInfo);
        return 2.5;
    }

    renderRound(player, true, roundInfo);

    std::vector<bool> playerBusted;
    playerBusted.reserve(2);
    for (size_t i = 0; i < playerHand.size(); ++i) {
        bool busted = playerTurn(player, i);
        playerBusted.push_back(busted);
    }

    bool allDead = true;
    for (size_t i = 0; i < playerHand.size(); ++i) {
        if (!playerBusted[i] && !surrendered[i]) {
            allDead = false;
            break;
        }
    }

    if (allDead) {
        roundInfo = "All your hands are either busted or surrendered.";
        renderRound(player, false, roundInfo);
        return 0.0;
    }

    dealerHand.push_back(drawCard());
    int dealerSum = dealerHand[0].value + dealerHand[1].value;

    roundInfo = "Dealer draws a second card.";
    renderRound(player, false, roundInfo);

    if (dealerSum == 21) {
        roundInfo = "Dealer has Blackjack! You lose your bet.";
        renderRound(player, false, roundInfo);
        lastScore = 0;
        return 0.0;
    }

    while (dealerSum < 17) {
        Card newCard = drawCard();
        dealerHand.push_back(newCard);
        dealerSum += newCard.value;

        roundInfo = "Dealer draws a card.";
        renderRound(player, false, roundInfo);
    }

    roundInfo = "Dealer stands on " + std::to_string(dealerSum) + ".";
    renderRound(player, false, roundInfo);

    if (dealerSum > 21) {
        roundInfo = "Dealer busted! All non-busted hands win even money.";
        renderRound(player, false, roundInfo);

        double totalMultiplier = 0.0;
        for (size_t i = 0; i < playerHand.size(); ++i) {
            if (!playerBusted[i] && !surrendered[i]) {
                totalMultiplier += 2.0;  // Win: ×2
            }
        }

        return totalMultiplier;
    }

    double totalMultiplier = 0.0;

    for (size_t i = 0; i < playerHand.size(); ++i) {
        if (playerBusted[i] || surrendered[i]) {
            continue;
        }

        int sum = 0;
        for (auto &c : playerHand[i]) sum += c.value;


        int playerRange = abs(21 - sum);
        int dealerRange = abs(21 - dealerSum);

        if (playerRange < dealerRange) {
            roundInfo = "One of your hands wins!";
            totalMultiplier += 2.0;
        } else if (playerRange == dealerRange) {
            roundInfo = "One of your hands pushes.";
            totalMultiplier += 1.0;
        } else {
            roundInfo = "One of your hands loses.";
        }

        renderRound(player, false, roundInfo);
    }

    return totalMultiplier;
}

bool BlackjackGame::playerTurn(Player &player, size_t handIndex) {
    auto &hand = playerHand[handIndex];
    std::string statusMessage = "Playing hand " + std::to_string(handIndex + 1) + ".";

    bool firstAction = (hand.size() == 2);

    while (true) {
        renderRound(player, true, statusMessage);
        statusMessage.clear();

        int option = ui.askChoice(TextRes::BLACKJACK_ROUND_OPTIONS_TITLE,
                                  TextRes::BLACKJACK_ROUND_OPTIONS,
                                  false);

        auto choice = static_cast<BlackjackRoundOptions>(option);

        switch (choice) {
            case BlackjackRoundOptions::HIT: {
                Card newCard = drawCard();
                hand.push_back(newCard);

                int sum = 0;
                for (const auto &card : hand) sum += card.value;

                if (sum > 21) {
                    statusMessage = "You drew " + newCard.rank + " and busted!";
                    renderRound(player, false, statusMessage);
                    return true;
                } else {
                    statusMessage = "You drew " + newCard.rank + ".";
                    firstAction = false;
                    continue;
                }
            }

            case BlackjackRoundOptions::STAND: {
                statusMessage = "You chose to stand.";
                renderRound(player, false, statusMessage);
                return false;
            }

            case BlackjackRoundOptions::DOUBLE_DOWN: {
                if (!firstAction) {
                    statusMessage = "You can only double-down on your first action!";
                    continue;
                }

                int betForThisHand = handBets[handIndex];
                if (betForThisHand * 2 > player.getBalance()) {
                    statusMessage = "Insufficient balance to double-down!";
                    continue;
                }

                player.updateBalance(-betForThisHand);
                handBets[handIndex] *= 2;

                Card newCard = drawCard();
                hand.push_back(newCard);

                int sum = 0;
                for (const auto &card : hand) sum += card.value;

                if (sum > 21) {
                    statusMessage = "You doubled-down, drew " + newCard.rank + " and busted!";
                    renderRound(player, false, statusMessage);
                    return true;
                }

                statusMessage = "You doubled-down and drew " + newCard.rank + ".";
                renderRound(player, false, statusMessage);
                return false;
            }

            case BlackjackRoundOptions::SPLIT: {
                if (!firstAction) {
                    statusMessage = "You can only split on your first action!";
                    continue;
                }

                if (hand.size() != 2) {
                    statusMessage = "You can only split your initial two cards";
                    continue;
                }

                if (hand[0].value != hand[1].value) {
                    statusMessage = "You can only split a pair!";
                    continue;
                }

                int betForThisHand = handBets[handIndex];
                if (betForThisHand > player.getBalance()) {
                    statusMessage = "Insufficient balance to split!";
                    continue;
                }

                Card cardMoved = hand.back();
                hand.pop_back();

                std::vector<Card> newHand;
                newHand.push_back(cardMoved);

                hand.push_back(drawCard());
                newHand.push_back(drawCard());

                player.updateBalance(-betForThisHand);

                handBets.push_back(betForThisHand);
                surrendered.push_back(false);
                playerHand.push_back(std::move(newHand));

                statusMessage = "You split your hand.";
                firstAction = false;
                continue;
            }

            case BlackjackRoundOptions::SURRENDER: {
                if (!firstAction) {
                    statusMessage = "You can only surrender on your first action!";
                    continue;
                }

                int betForThisHand = handBets[handIndex];
                int refund = betForThisHand / 2;
                player.updateBalance(refund);

                surrendered[handIndex] = true;
                handBets[handIndex] = 0;

                statusMessage = "You surrendered this hand and got half your bet back.";
                renderRound(player, false, statusMessage);
                return true;
            }

            default: {
                statusMessage = "Invalid choice, please try again.";
                continue;
            }
        }
    }
}

GameState BlackjackGame::playRound(Player &player) {
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

        switch (static_cast<BlackjackOptions>(option)) {
            case BlackjackOptions::PLAY_ROUND: {
                try {
                    if (!player.hasActiveBet()) {
                        player.placeBet(selectedBet);
                    }

                    double multiplier = handleRound(player);

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
            case BlackjackOptions::CHANGE_BET: {
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
                    errorMessage = "Invalid bet amount. Keeping previous bet.";
                } else {
                    selectedBet = newBet;
                    lastScore = -1;
                }

                break;
            }
            case BlackjackOptions::VIEW_PAYOUTS: {
                displayPayouts();
                break;
            }
            case BlackjackOptions::RULES: {
                displayRules();
                break;
            }
            case BlackjackOptions::EXIT_TO_GAME_MENU: {
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
            case BlackjackOptions::EXIT: {
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