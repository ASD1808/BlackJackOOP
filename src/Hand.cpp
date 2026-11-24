#include "Hand.h"
#include <stdexcept>

Hand::Hand(int capacity) : capacity(capacity), count(0) {
    cards = new Card[capacity];
}

Hand::Hand(const Hand& other) : capacity(other.capacity), count(other.count) {
    cards = new Card[capacity];
    for (int i = 0; i < count; ++i) {
        cards[i] = other.cards[i];
    }
}

Hand& Hand::operator=(const Hand& other) {
    if (this != &other) {
        delete[] cards;
        capacity = other.capacity;
        count = other.count;
        cards = new Card[capacity];
        for (int i = 0; i < count; ++i) {
            cards[i] = other.cards[i];
        }
    }
    return *this;
}

Hand::~Hand() {
    delete[] cards;
}

void Hand::addCard(const Card& card) {
    if (count < capacity) {
        cards[count++] = card;
    }
}

void Hand::clear() {
    count = 0;
}

int Hand::getCount() const {
    return count;
}

const Card& Hand::getCard(int index) const {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index invalid in Hand");
    }
    return cards[index];
}

int Hand::calculateScore() const {
    int score = 0;
    int aces = 0;

    for (int i = 0; i < count; ++i) {
        score += cards[i].getValue();
        if (cards[i].isAce()) {
            ++aces;
        }
    }

    while (score > 21 && aces > 0) {
        score -= 10;
        --aces;
    }

    return score;
}

bool Hand::isBust() const {
    return calculateScore() > 21;
}

bool Hand::isBlackjack() const {
    return count == 2 && calculateScore() == 21;
}

std::ostream& operator<<(std::ostream& os, const Hand& hand) {
    for (int i = 0; i < hand.count; ++i) {
        os << hand.cards[i];
        if (i < hand.count - 1) {
            os << ", ";
        }
    }
    os << " (Score: " << hand.calculateScore() << ")";
    return os;
}