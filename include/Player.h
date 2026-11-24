#pragma once

#include "Card.h"
#include "Hand.h"
#include <iostream>
#include <string>

class Player {
private:
    std::string name;
    Hand hand;
    double balance;
    double currentBet;

public:
    Player(std::string name, double startingBalance);

    const std::string& getName() const;
    double getBalance() const;
    double getCurrentBet() const;
    const Hand& getHand() const;

    bool placeBet(double amount);
    void winBet();
    void winBlackjack(double payoutMultiplier);
    void pushBet();
    void loseBet();

    void receiveCard(const Card& card);
    void resetHand();

    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};