#include "Game.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

Game::Game(const std::string& playerName, const std::string& deckPath, const std::string& rulesPath)
    : deck(deckPath),
      player(playerName, 0.0),
      dealer("Dealer", 0.0),
      minBet(10.0),
      maxBet(500.0),
      dealerStandOn(17),
      blackjackPayout(1.5),
      roundActive(false),
      lastOutcome("Waiting for bet.") {
    loadRules(rulesPath);
}

void Game::loadRules(const std::string& rulesPath) {
    std::ifstream file(rulesPath);
    if (!file.is_open()) {
        throw std::runtime_error("Nu s-a putut deschide fisierul de reguli: " + rulesPath);
    }

    nlohmann::json data;
    file >> data;

    minBet = data.at("min_bet").get<double>();
    maxBet = data.at("max_bet").get<double>();
    dealerStandOn = data.at("dealer_stand_on").get<int>();
    blackjackPayout = data.at("blackjack_payout").get<double>();

    double startingBalance = data.at("starting_balance").get<double>();
    player = Player(player.getName(), startingBalance);
}

bool Game::startRound(double betAmount) {
    if (betAmount < minBet || betAmount > maxBet) {
        lastOutcome = "Bet must be between min and max limits.";
        return false;
    }
    if (!player.placeBet(betAmount)) {
        lastOutcome = "Insufficient balance for this bet.";
        return false;
    }

    if (deck.remainingCards() < 15) {
        deck.reset();
    }

    player.resetHand();
    dealer.resetHand();

    player.receiveCard(deck.drawCard());
    dealer.receiveCard(deck.drawCard());
    player.receiveCard(deck.drawCard());
    dealer.receiveCard(deck.drawCard());

    if (player.getHand().isBlackjack() && dealer.getHand().isBlackjack()) {
        player.pushBet();
        roundActive = false;
        lastOutcome = "Both have Blackjack! Push.";
    } else if (player.getHand().isBlackjack()) {
        player.winBlackjack(blackjackPayout);
        roundActive = false;
        lastOutcome = "Blackjack! You win 3:2!";
    } else if (dealer.getHand().isBlackjack()) {
        player.loseBet();
        roundActive = false;
        lastOutcome = "Dealer has Blackjack! You lose.";
    } else {
        roundActive = true;
        lastOutcome = "Round in progress.";
    }

    return true;
}

void Game::playerHit() {
    if (!roundActive) {
        return;
    }

    player.receiveCard(deck.drawCard());

    if (player.getHand().isBust()) {
        player.loseBet();
        roundActive = false;
        lastOutcome = "Bust! You went over 21.";
    } else if (player.getHand().calculateScore() == 21) {
        playerStand();
    }
}

void Game::playerStand() {
    if (!roundActive) {
        return;
    }

    while (dealer.getHand().calculateScore() < dealerStandOn) {
        dealer.receiveCard(deck.drawCard());
    }

    resolveRound();
    roundActive = false;
}

void Game::resolveRound() {
    int playerScore = player.getHand().calculateScore();
    int dealerScore = dealer.getHand().calculateScore();

    if (dealer.getHand().isBust()) {
        player.winBet();
        lastOutcome = "Dealer busts! You win!";
    } else if (playerScore > dealerScore) {
        player.winBet();
        lastOutcome = "You win!";
    } else if (playerScore < dealerScore) {
        player.loseBet();
        lastOutcome = "Dealer wins!";
    } else {
        player.pushBet();
        lastOutcome = "Push (Tie)!";
    }
}

bool Game::isRoundActive() const {
    return roundActive;
}

double Game::getMinBet() const {
    return minBet;
}

double Game::getMaxBet() const {
    return maxBet;
}

const std::string& Game::getLastOutcome() const {
    return lastOutcome;
}

const Player& Game::getPlayer() const {
    return player;
}

const Player& Game::getDealer() const {
    return dealer;
}

const Deck& Game::getDeck() const {
    return deck;
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "=== Blackjack Table ===\n";
    os << game.deck << "\n";
    if (game.roundActive && game.dealer.getHand().getCount() > 0) {
        os << game.dealer.getName() << " -> " << game.dealer.getHand().getCard(0) << ", [Hidden Card]\n";
    } else {
        os << game.dealer << "\n";
    }
    os << game.player;
    return os;
}