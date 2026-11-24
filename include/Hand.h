#pragma once

#include "Card.h"
#include <iostream>

class Hand {
private:
    Card* cards;
    int capacity;
    int count;

public:
    explicit Hand(int capacity = 12);

    Hand(const Hand& other);
    Hand& operator=(const Hand& other);
    ~Hand();

    void addCard(const Card& card);
    void clear();

    int getCount() const;
    const Card& getCard(int index) const;

    int calculateScore() const;
    bool isBust() const;
    bool isBlackjack() const;

    friend std::ostream& operator<<(std::ostream& os, const Hand& hand);
};