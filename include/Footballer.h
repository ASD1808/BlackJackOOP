#pragma once

#include <iostream>
#include <string>

class Footballer {
private:
    std::string name;
    std::string position;
    int attack;
    int defense;
    int stamina;
    double price;

public:
    Footballer();
    Footballer(std::string name, std::string position,
               int attack, int defense, int stamina, double price);

    const std::string& getName() const;
    const std::string& getPosition() const;
    int getStamina() const;
    double getPrice() const;

    double getEffectiveAttack() const;
    double getEffectiveDefense() const;

    void train(int attackBoost, int defenseBoost, int staminaCost);
    void recover(int staminaGain);
    void consumeStamina(int amount);

    friend std::ostream& operator<<(std::ostream& os, const Footballer& player);
};