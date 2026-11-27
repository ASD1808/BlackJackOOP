#pragma once

#include "Footballer.h"
#include <iostream>

class Squad {
private:
    Footballer* players;
    int count;
    int capacity;

    void resize(int newCapacity);

public:
    explicit Squad(int initialCapacity = 4);
    Squad(const Squad& other);
    Squad& operator=(const Squad& other);
    ~Squad();

    void addPlayer(const Footballer& player);
    int getCount() const;
    const Footballer& getPlayer(int index) const;

    double calculateAttackRating() const;
    double calculateDefenseRating() const;
    double calculateAverageStamina() const;

    void trainAll(int attackBoost, int defenseBoost, int staminaCost);
    void restAll(int staminaGain);
    void playMatchFatigue(int baseFatigue);

    friend std::ostream& operator<<(std::ostream& os, const Squad& squad);
};