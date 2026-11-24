#pragma once

#include <iostream>
#include <string>

class Card {
private:
    std::string rank;
    std::string suit;
    int value;

public:
    Card();
    Card(std::string rank, std::string suit, int value);

    int getValue() const;
    const std::string& getRank() const;
    const std::string& getSuit() const;
    bool isAce() const;

    friend std::ostream& operator<<(std::ostream& os, const Card& card);
};