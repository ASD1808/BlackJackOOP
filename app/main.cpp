#include "Game.h"
#include <iostream>
#include <string>

int main() {
    try {
        std::string playerName;
        std::cout << "Enter player name: ";
        if (!(std::cin >> playerName)) {
            playerName = "Player1";
        }

        Game game(playerName);
        std::cout << "Welcome, " << game.getPlayer().getName() << "!\n";
        std::cout << "Table limits: " << game.getMinBet() << " - " << game.getMaxBet() << "\n";

        while (game.getPlayer().getBalance() >= game.getMinBet()) {
            std::cout << "\nCurrent balance: " << game.getPlayer().getBalance() << "\n";
            std::cout << "Enter bet amount (or 0 to quit): ";

            double bet = 0.0;
            if (!(std::cin >> bet) || bet == 0.0) {
                break;
            }

            if (!game.startRound(bet)) {
                std::cout << game.getLastOutcome() << "\n";
                continue;
            }

            while (game.isRoundActive()) {
                std::cout << "\n" << game << "\n";
                std::cout << "Current bet: " << game.getPlayer().getCurrentBet() << "\n";
                std::cout << "Choose action - (h)it or (s)tand: ";

                char action = 's';
                if (!(std::cin >> action)) {
                    action = 's';
                }

                if (action == 'h' || action == 'H') {
                    game.playerHit();
                } else if (action == 's' || action == 'S') {
                    game.playerStand();
                } else {
                    std::cout << "Invalid choice. Please enter 'h' or 's'.\n";
                }
            }

            std::cout << "\n" << game << "\n";
            std::cout << "Result: " << game.getLastOutcome() << "\n";
        }

        if (game.getPlayer().getHand().getCount() > 0) {
            const Card& firstCard = game.getPlayer().getHand().getCard(0);
            std::cout << "\nLast round first card: " << firstCard.getRank()
                      << " (" << firstCard.getSuit() << ")\n";

            Hand copiedHand(game.getPlayer().getHand());
            Hand assignedHand;
            assignedHand = copiedHand;
            std::cout << "Hand copy check: " << assignedHand << "\n";
        }

        std::cout << "Last dealer score (" << game.getDealer().getName() << "): "
                  << game.getDealer().getHand().calculateScore() << "\n";

        std::cout << "Deck empty: " << (game.getDeck().isEmpty() ? "yes" : "no") << "\n";
        std::cout << "Final balance for " << game.getPlayer().getName()
                  << ": " << game.getPlayer().getBalance() << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
