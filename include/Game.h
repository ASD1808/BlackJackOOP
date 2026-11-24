#pragma once

#include "Deck.h"
#include "Player.h"
#include <iostream>
#include <string>

class Game {
private:
    Deck deck;
    Player player;
    Player dealer;
    double minBet;
    double maxBet;
    int dealerStandOn;
    double blackjackPayout;
    bool roundActive;
    std::string lastOutcome;

    void loadRules(const std::string& rulesPath);
    void resolveRound();

public:
    Game(const std::string& playerName,
         const std::string& deckPath = "assets/deck.json",
         const std::string& rulesPath = "assets/rules.json");

    bool startRound(double betAmount);
    void playerHit();
    void playerStand();

    bool isRoundActive() const;
    double getMinBet() const;
    double getMaxBet() const;
    const std::string& getLastOutcome() const;
    const Player& getPlayer() const;
    const Player& getDealer() const;
    const Deck& getDeck() const;

    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};