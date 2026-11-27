#include "Team.h"
#include <iomanip>
#include <utility>

Team::Team(std::string name, double startingBudget)
    : name(std::move(name)),
      budget(startingBudget),
      points(0),
      goalsFor(0),
      goalsAgainst(0) {}

const std::string& Team::getName() const {
    return name;
}

double Team::getBudget() const {
    return budget;
}

int Team::getPoints() const {
    return points;
}

int Team::getGoalDifference() const {
    return goalsFor - goalsAgainst;
}

const Squad& Team::getSquad() const {
    return squad;
}

void Team::addInitialPlayer(const Footballer& player) {
    squad.addPlayer(player);
}

bool Team::buyPlayer(const Footballer& player, int maxSquadSize) {
    if (squad.getCount() >= maxSquadSize || budget < player.getPrice()) {
        return false;
    }
    budget -= player.getPrice();
    squad.addPlayer(player);
    return true;
}

bool Team::trainTeam(double cost) {
    if (budget < cost) {
        return false;
    }
    budget -= cost;
    squad.trainAll(3, 3, 15);
    return true;
}

void Team::restTeam() {
    squad.restAll(25);
}

void Team::applyMatchResult(int scored, int conceded, double reward) {
    goalsFor += scored;
    goalsAgainst += conceded;

    if (scored > conceded) {
        points += 3;
    } else if (scored == conceded) {
        points += 1;
    }

    budget += reward;
    squad.playMatchFatigue(20);
}

std::ostream& operator<<(std::ostream& os, const Team& team) {
    os << "Team: " << team.name
       << " | Budget: $" << team.budget
       << " | Points: " << team.points
       << " | GD: " << team.getGoalDifference()
       << " | ATK: " << std::fixed << std::setprecision(1) << team.squad.calculateAttackRating()
       << " | DEF: " << team.squad.calculateDefenseRating()
       << " | Avg STA: " << team.squad.calculateAverageStamina() << "%\n"
       << team.squad;
    return os;
}