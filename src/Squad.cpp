#include "Squad.h"
#include <cstddef>
#include <stdexcept>

Squad::Squad(int initialCapacity)
    : players(nullptr),
      count(0),
      capacity(initialCapacity > 0 ? initialCapacity : 4) {
    players = new Footballer[static_cast<std::size_t>(capacity)];
}

Squad::Squad(const Squad& other)
    : players(new Footballer[static_cast<std::size_t>(other.capacity)]),
      count(other.count),
      capacity(other.capacity) {
    for (int i = 0; i < count; ++i) {
        players[i] = other.players[i];
    }
}

Squad& Squad::operator=(const Squad& other) {
    if (this == &other) {
        return *this;
    }

    auto* newPlayers = new Footballer[static_cast<std::size_t>(other.capacity)];
    for (int i = 0; i < other.count; ++i) {
        newPlayers[i] = other.players[i];
    }

    delete[] players;
    players = newPlayers;
    count = other.count;
    capacity = other.capacity;
    return *this;
}

Squad::~Squad() {
    delete[] players;
}

void Squad::resize(int newCapacity) {
    auto* newPlayers = new Footballer[static_cast<std::size_t>(newCapacity)];
    for (int i = 0; i < count; ++i) {
        newPlayers[i] = players[i];
    }
    delete[] players;
    players = newPlayers;
    capacity = newCapacity;
}

void Squad::addPlayer(const Footballer& player) {
    if (count >= capacity) {
        resize(capacity * 2);
    }
    players[count++] = player;
}

int Squad::getCount() const {
    return count;
}

const Footballer& Squad::getPlayer(int index) const {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index invalid in lotul echipei.");
    }
    return players[index];
}

double Squad::calculateAttackRating() const {
    if (count == 0) {
        return 0.0;
    }

    double total = 0.0;
    for (int i = 0; i < count; ++i) {
        double weight = 1.0;
        if (players[i].getPosition() == "FWD") {
            weight = 1.15;
        } else if (players[i].getPosition() == "MID") {
            weight = 1.05;
        }
        total += players[i].getEffectiveAttack() * weight;
    }
    return total / static_cast<double>(count);
}

double Squad::calculateDefenseRating() const {
    if (count == 0) {
        return 0.0;
    }

    double total = 0.0;
    for (int i = 0; i < count; ++i) {
        double weight = 1.0;
        if (players[i].getPosition() == "GK") {
            weight = 1.20;
        } else if (players[i].getPosition() == "DEF") {
            weight = 1.15;
        }
        total += players[i].getEffectiveDefense() * weight;
    }
    return total / static_cast<double>(count);
}

double Squad::calculateAverageStamina() const {
    if (count == 0) {
        return 0.0;
    }

    double total = 0.0;
    for (int i = 0; i < count; ++i) {
        total += static_cast<double>(players[i].getStamina());
    }
    return total / static_cast<double>(count);
}

void Squad::trainAll(int attackBoost, int defenseBoost, int staminaCost) {
    for (int i = 0; i < count; ++i) {
        players[i].train(attackBoost, defenseBoost, staminaCost);
    }
}

void Squad::restAll(int staminaGain) {
    for (int i = 0; i < count; ++i) {
        players[i].recover(staminaGain);
    }
}

void Squad::playMatchFatigue(int baseFatigue) {
    for (int i = 0; i < count; ++i) {
        int fatigue = baseFatigue;
        if (players[i].getPosition() == "GK") {
            fatigue = baseFatigue / 2;
        }
        players[i].consumeStamina(fatigue);
    }
}

std::ostream& operator<<(std::ostream& os, const Squad& squad) {
    for (int i = 0; i < squad.count; ++i) {
        os << "  " << (i + 1) << ". " << squad.players[i] << "\n";
    }
    return os;
}