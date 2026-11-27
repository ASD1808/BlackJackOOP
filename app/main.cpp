#include "League.h"
#include <cstddef>
#include <iostream>

int main() {
    try {
        League league;
        std::cout << "Welcome to Mini Football Manager!\n";
        std::cout << league.getLastReport() << "\n";

        int option = -1;
        while (option != 0) {
            std::cout << "\n--- Menu (" << league.getPlayerTeam().getName()
                      << " | Budget: $" << league.getPlayerTeam().getBudget() << ") ---\n"
                      << "1. View team & league status\n"
                      << "2. View transfer market (" << league.getMarketSize() << " available)\n"
                      << "3. Buy player from market\n"
                      << "4. Train team (Cost: $" << league.getTrainingCost() << ")\n"
                      << "5. Rest team\n"
                      << "6. Play next match" << (league.hasNextMatch() ? "" : " [Season finished]") << "\n"
                      << "7. View league standings\n"
                      << "0. Exit\n"
                      << "Choose an option: ";

            if (!(std::cin >> option)) {
                break;
            }

            switch (option) {
                case 1:
                    std::cout << "\n" << league;
                    break;
                case 2:
                    std::cout << "\n";
                    league.displayMarket(std::cout);
                    break;
                case 3: {
                    league.displayMarket(std::cout);
                    std::cout << "Enter player number to sign: ";
                    std::size_t choice = 0;
                    if (!(std::cin >> choice)) {
                        option = 0;
                        break;
                    }
                    std::size_t marketIndex = (choice > 0) ? (choice - 1) : league.getMarketSize();
                    league.buyFromMarket(marketIndex);
                    std::cout << league.getLastReport() << "\n";
                    break;
                }
                case 4:
                    league.trainPlayerTeam();
                    std::cout << league.getLastReport() << "\n";
                    break;
                case 5:
                    league.restPlayerTeam();
                    std::cout << league.getLastReport() << "\n";
                    break;
                case 6:
                    league.playNextMatch();
                    std::cout << "\n" << league.getLastReport() << "\n";
                    break;
                case 7:
                    std::cout << "\n";
                    league.displayStandings(std::cout);
                    break;
                case 0:
                    std::cout << "Exiting Mini Football Manager.\n";
                    break;
                default:
                    std::cout << "Invalid option. Please try again.\n";
                    break;
            }
        }

        // Verificare explicita pentru Regula celor 3 (Squad) si metodele de acces
        const Squad& currentSquad = league.getPlayerTeam().getSquad();
        if (currentSquad.getCount() > 0) {
            const Footballer& captain = currentSquad.getPlayer(0);
            std::cout << "\nTeam captain: " << captain.getName()
                      << " [" << captain.getPosition() << "] "
                      << "STA: " << captain.getStamina() << "% "
                      << "Value: $" << captain.getPrice() << "\n";

            Squad copiedSquad(currentSquad);
            Squad assignedSquad;
            assignedSquad = copiedSquad;
            std::cout << "Squad copy check (" << assignedSquad.getCount() << " players):\n"
                      << assignedSquad;
        }

        std::cout << "\nFinal standings:\n";
        league.displayStandings(std::cout);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}