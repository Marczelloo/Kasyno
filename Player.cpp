//
// Created by moskw on 08.10.2025.
//

#include "Player.h"

#include <algorithm>
#include <stdexcept>

Player::Player()
    : name("Guest")
    , balance(0)
    , winnings(0)
    , currentBet(0) {
}

Player::Player(const std::string& name, const int &balance)
    : name(name)
    , balance(balance)
    , winnings(0)
    , currentBet(0) {

    std::string trimmedName = name;
    trimmedName.erase(
        std::remove_if(trimmedName.begin(), trimmedName.end(), ::isspace()),
        trimmedName.end()
    );

    if (trimmedName.empty()) {
        throw std::invalid_argument("Player::Player: name cannot be empty");
    }

    if (balance < 0) {
        throw std::invalid_argument("Player::Player: balance (" + std::to_string(balance) + ") cannot be negative");
    }
}

std::string Player::getName() const {
    return this->name;
}

int Player::getBalance() const {
    return this->balance;
}

int Player::getWinnings() const {
    return this->winnings;
}

int Player::getCurrentBet() const {
    return this->currentBet;
}

bool Player::hasActiveBet() const {
    return currentBet > 0;
}

bool Player::canAffordBet(int amount) const {
    return amount >= 0 && balance >= amount;
}


void Player::setName(const std::string& newName) {
    std::string trimmedName = newName;
    trimmedName.erase(
        std::remove_if(trimmedName.begin(), trimmedName.end(), ::isspace),
        trimmedName.end()
    );

    if (trimmedName.empty()) {
        throw std::invalid_argument("Player::setName: name cannot be empty");
    }

    name = newName;
}

void Player::setBalance(const int& newBalance) {
    if (newBalance < 0) {
        throw std::invalid_argument(
            "Player::setBalance: balance (" + std::to_string(newBalance) +
            ") cannot be negative"
        );
    }

    balance = newBalance;
}

void Player::setWinnings(int newWinnings) {
    winnings = newWinnings;
}

void Player::setCurrentBet(int newBet) {
    if (newBet < 0) {
        throw std::invalid_argument(
            "Player::setCurrentBet: bet (" + std::to_string(newBet) +") cannot be negative");
    }

    if (newBet > balance) {
        throw std::invalid_argument(
            "Player::setCurrentBet: bet (" + std::to_string(newBet) +
            ") exceeds balance (" + std::to_string(balance) + ")"
        );
    }

    currentBet = newBet;
}

void Player::placeBet(int amount) {
    if (amount <= 0) {
        throw std::invalid_argument(
            "Player::placeBet: amount (" + std::to_string(amount) + ") must be positive"
        );
    }

    if (currentBet > 0) {
        throw std::logic_error(
            "Player::placeBet: player already has an active bet " + std::to_string(currentBet)
        );
    }

    if (amount > balance) {
        throw std::invalid_argument(
            "Player::placeBet: amount (" + std::to_string(amount) +") exceeds balance (" + std::to_string(balance) + ")"
        );
    }

    currentBet = amount;
    balance -= amount;
}

void Player::winBet(double multiplayer) {
    if (currentBet <= 0) {
        throw std::logic_error("Player::winBet: no active bet to win");
    }

    if (multiplayer <= 0.0) {
        throw std::invalid_argument(
           "Player::winBet: multiplier (" + std::to_string(multiplier) +
           ") must be positive"
       );
    }

    int payout = static_cast<int>(currentBet * multiplayer);

    balance += payout;

    int netWin = payout - currentBet;
    winnings += netWin;

    currentBet = 0;
}

void Player::loseBet() {
    if (currentBet <= 0) {
        throw std::logic_error("Player::loseBet: no active bet to lose");
    }

    winnings -= currentBet;

    currentBet = 0;
}

void Player::cancelBet() {
    if (currentBet <= 0) {
        throw std::logic_error("Player::cancelBet: no active bet to cancel");
    }

    balance += currentBet;

    currentBet = 0;
}

void Player::updateBalance(int amount) {
    int newBalance = balance + amount;

    if (newBalance < 0) {
        throw std::invalid_argument(
            "Player::updateBalance: resulting balance (" +
            std::to_string(newBalance) + ") would be negative"
        );
    }

    balance = newBalance;
}

void Player::resetStats() {
    winnings = 0;
    currentBet = 0;
}

void Player::reset() {
    name = "Guest";
    balance = 0;
    winnings = 0;
    currentBet = 0;
}

