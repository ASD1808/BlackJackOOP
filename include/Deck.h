#pragma once

#include "Card.h"
#include <iostream>
#include <string>
#include <vector>

class Deck {
private:
    std::vector<Card> cards;
    std::string sourceFile;

    void loadFromFile();

public:
    explicit Deck(std::string filePath = "assets/deck.json");

    void shuffle();
    void reset();
    Card drawCard();

    int remainingCards() const;
    bool isEmpty() const;

    friend std::ostream& operator<<(std::ostream& os, const Deck& deck);
};