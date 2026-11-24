#include "Player.h"
#include <utility>

Player::Player(std::string name, double startingBalance)
    : name(std::move(name)), balance(startingBalance), currentBet(0.0) {}

const std::string& Player::getName() const {
    return name;
}

double Player::getBalance() const {
    return balance;
}

double Player::getCurrentBet() const {
    return currentBet;
}

const Hand& Player::getHand() const {
    return hand;
}

bool Player::placeBet(double amount) {
    if (amount <= 0.0 || amount > balance) {
        return false;
    }
    balance -= amount;
    currentBet = amount;
    return true;
}

void Player::winBet() {
    balance += currentBet * 2.0;
    currentBet = 0.0;
}

void Player::winBlackjack(double payoutMultiplier) {
    balance += currentBet * (1.0 + payoutMultiplier);
    currentBet = 0.0;
}

void Player::pushBet() {
    balance += currentBet;
    currentBet = 0.0;
}

void Player::loseBet() {
    currentBet = 0.0;
}

void Player::receiveCard(const Card& card) {
    hand.addCard(card);
}

void Player::resetHand() {
    hand.clear();
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << player.name << " [Balance: " << player.balance << "] -> " << player.hand;
    return os;
}