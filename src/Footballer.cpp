#include "Footballer.h"
#include <algorithm>
#include <utility>

Footballer::Footballer() : attack(0), defense(0), stamina(100), price(0.0) {}

Footballer::Footballer(std::string name, std::string position,
                       int attack, int defense, int stamina, double price)
    : name(std::move(name)),
      position(std::move(position)),
      attack(std::clamp(attack, 1, 99)),
      defense(std::clamp(defense, 1, 99)),
      stamina(std::clamp(stamina, 0, 100)),
      price(price) {}

const std::string& Footballer::getName() const {
    return name;
}

const std::string& Footballer::getPosition() const {
    return position;
}

int Footballer::getStamina() const {
    return stamina;
}

double Footballer::getPrice() const {
    return price;
}

double Footballer::getEffectiveAttack() const {
    double factor = 0.5 + 0.5 * (static_cast<double>(stamina) / 100.0);
    return static_cast<double>(attack) * factor;
}

double Footballer::getEffectiveDefense() const {
    double factor = 0.5 + 0.5 * (static_cast<double>(stamina) / 100.0);
    return static_cast<double>(defense) * factor;
}

void Footballer::train(int attackBoost, int defenseBoost, int staminaCost) {
    attack = std::clamp(attack + attackBoost, 1, 99);
    defense = std::clamp(defense + defenseBoost, 1, 99);
    consumeStamina(staminaCost);
}

void Footballer::recover(int staminaGain) {
    stamina = std::clamp(stamina + staminaGain, 0, 100);
}

void Footballer::consumeStamina(int amount) {
    stamina = std::clamp(stamina - amount, 0, 100);
}

std::ostream& operator<<(std::ostream& os, const Footballer& player) {
    os << player.name << " [" << player.position << "] "
       << "ATK:" << player.attack
       << " DEF:" << player.defense
       << " STA:" << player.stamina << "%"
       << " ($" << player.price << ")";
    return os;
}