#include "Card.h"
#include <utility>

Card::Card() : rank(""), suit(""), value(0) {}

Card::Card(std::string rank, std::string suit, int value)
    : rank(std::move(rank)), suit(std::move(suit)), value(value) {}

int Card::getValue() const {
    return value;
}

const std::string& Card::getRank() const {
    return rank;
}

const std::string& Card::getSuit() const {
    return suit;
}

bool Card::isAce() const {
    return rank == "Ace";
}

std::ostream& operator<<(std::ostream& os, const Card& card) {
    os << card.rank << " of " << card.suit;
    return os;
}