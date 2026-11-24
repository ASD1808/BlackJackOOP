#include "Deck.h"
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>
#include <random>
#include <stdexcept>
#include <utility>

Deck::Deck(std::string filePath) : sourceFile(std::move(filePath)) {
    loadFromFile();
    shuffle();
}

void Deck::loadFromFile() {
    std::ifstream file(sourceFile);
    if (!file.is_open()) {
        throw std::runtime_error("Nu s-a putut deschide fisierul pachetului: " + sourceFile);
    }

    nlohmann::json data;
    file >> data;

    cards.clear();
    const auto& suits = data.at("suits");
    const auto& ranks = data.at("ranks");

    for (const auto& suit : suits) {
        for (const auto& rankObj : ranks) {
            std::string rankName = rankObj.at("name").get<std::string>();
            int value = rankObj.at("value").get<int>();
            cards.emplace_back(rankName, suit.get<std::string>(), value);
        }
    }
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(cards.begin(), cards.end(), generator);
}

void Deck::reset() {
    loadFromFile();
    shuffle();
}

Card Deck::drawCard() {
    if (cards.empty()) {
        reset();
    }
    Card top = cards.back();
    cards.pop_back();
    return top;
}

int Deck::remainingCards() const {
    return static_cast<int>(cards.size());
}

bool Deck::isEmpty() const {
    return cards.empty();
}

std::ostream& operator<<(std::ostream& os, const Deck& deck) {
    os << "Deck (" << deck.cards.size() << " cards remaining)";
    return os;
}