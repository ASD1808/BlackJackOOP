#include "League.h"
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>
#include <random>
#include <sstream>
#include <stdexcept>

namespace {

Footballer parseFootballer(const nlohmann::json& j) {
    return Footballer(
        j.at("name").get<std::string>(),
        j.at("position").get<std::string>(),
        j.at("attack").get<int>(),
        j.at("defense").get<int>(),
        j.at("stamina").get<int>(),
        j.at("price").get<double>());
}

Team parseTeam(const nlohmann::json& j) {
    Team team(j.at("name").get<std::string>(), j.at("budget").get<double>());
    for (const auto& playerJson : j.at("players")) {
        team.addInitialPlayer(parseFootballer(playerJson));
    }
    return team;
}

int calculateGoals(double attackRating, double opponentDefenseRating, std::mt19937& rng) {
    std::uniform_int_distribution<int> luckDist(-10, 14);
    double advantage = (attackRating - opponentDefenseRating) + static_cast<double>(luckDist(rng));

    if (advantage < -12.0) {
        return 0;
    }
    if (advantage < 2.0) {
        return 1;
    }
    if (advantage < 15.0) {
        return 2;
    }
    if (advantage < 28.0) {
        return 3;
    }
    return 4;
}

} // namespace

League::League(const std::string& leaguePath, const std::string& marketPath)
    : playerTeam("Unnamed FC", 0.0),
      winReward(50000.0),
      drawReward(20000.0),
      trainingCost(15000.0),
      maxSquadSize(8),
      currentRound(0),
      lastReport("Season ready. Good luck!") {
    loadLeague(leaguePath);
    loadMarket(marketPath);
}

void League::loadLeague(const std::string& leaguePath) {
    std::ifstream file(leaguePath);
    if (!file.is_open()) {
        throw std::runtime_error("Nu s-a putut deschide fisierul campionatului: " + leaguePath);
    }

    nlohmann::json data;
    file >> data;

    winReward = data.at("win_reward").get<double>();
    drawReward = data.at("draw_reward").get<double>();
    trainingCost = data.at("training_cost").get<double>();
    maxSquadSize = data.at("max_squad_size").get<int>();

    playerTeam = parseTeam(data.at("player_team"));

    opponents.clear();
    for (const auto& oppJson : data.at("opponents")) {
        opponents.push_back(parseTeam(oppJson));
    }
}

void League::loadMarket(const std::string& marketPath) {
    std::ifstream file(marketPath);
    if (!file.is_open()) {
        throw std::runtime_error("Nu s-a putut deschide fisierul de transferuri: " + marketPath);
    }

    nlohmann::json data;
    file >> data;

    transferMarket.clear();
    for (const auto& playerJson : data.at("transfer_list")) {
        transferMarket.push_back(parseFootballer(playerJson));
    }
}

std::string League::simulateMatch(Team& home, Team& away) {
    std::random_device rd;
    std::mt19937 rng(rd());

    int homeGoals = calculateGoals(
        home.getSquad().calculateAttackRating(),
        away.getSquad().calculateDefenseRating(),
        rng);
    int awayGoals = calculateGoals(
        away.getSquad().calculateAttackRating(),
        home.getSquad().calculateDefenseRating(),
        rng);

    double homeReward = 0.0;
    double awayReward = 0.0;
    if (homeGoals > awayGoals) {
        homeReward = winReward;
    } else if (awayGoals > homeGoals) {
        awayReward = winReward;
    } else {
        homeReward = drawReward;
        awayReward = drawReward;
    }

    home.applyMatchResult(homeGoals, awayGoals, homeReward);
    away.applyMatchResult(awayGoals, homeGoals, awayReward);

    std::ostringstream oss;
    oss << home.getName() << " " << homeGoals << " - " << awayGoals << " " << away.getName();
    return oss.str();
}

bool League::hasNextMatch() const {
    return currentRound < opponents.size();
}

bool League::playNextMatch() {
    if (!hasNextMatch()) {
        lastReport = "All league matches have been played.";
        return false;
    }

    std::ostringstream report;
    report << "Round " << (currentRound + 1) << " results:\n";
    report << "  " << simulateMatch(playerTeam, opponents[currentRound]);

    // Simulam si meciul dintre ceilalti doi adversari ai etapei
    std::vector<std::size_t> otherIndices;
    for (std::size_t i = 0; i < opponents.size(); ++i) {
        if (i != currentRound) {
            otherIndices.push_back(i);
        }
    }
    if (otherIndices.size() >= 2) {
        report << "\n  " << simulateMatch(opponents[otherIndices[0]], opponents[otherIndices[1]]);
    }

    ++currentRound;
    lastReport = report.str();
    return true;
}

bool League::buyFromMarket(std::size_t marketIndex) {
    if (marketIndex >= transferMarket.size()) {
        lastReport = "Invalid transfer selection.";
        return false;
    }

    const Footballer& target = transferMarket[marketIndex];
    if (!playerTeam.buyPlayer(target, maxSquadSize)) {
        lastReport = "Cannot sign " + target.getName() + " (insufficient budget or full squad).";
        return false;
    }

    lastReport = "Signed " + target.getName() + " to " + playerTeam.getName() + "!";
    transferMarket.erase(transferMarket.begin() + static_cast<std::ptrdiff_t>(marketIndex));
    return true;
}

bool League::trainPlayerTeam() {
    if (!playerTeam.trainTeam(trainingCost)) {
        lastReport = "Not enough budget for training.";
        return false;
    }
    lastReport = playerTeam.getName() + " completed a training session (+3 ATK, +3 DEF, -15% STA).";
    return true;
}

void League::restPlayerTeam() {
    playerTeam.restTeam();
    for (auto& opp : opponents) {
        opp.restTeam();
    }
    lastReport = "Squad rested (+25% stamina recovered).";
}

void League::displayMarket(std::ostream& os) const {
    os << "=== Transfer Market ===\n";
    if (transferMarket.empty()) {
        os << "  No players available.\n";
        return;
    }
    for (std::size_t i = 0; i < transferMarket.size(); ++i) {
        os << "  " << (i + 1) << ". " << transferMarket[i] << "\n";
    }
}

void League::displayStandings(std::ostream& os) const {
    std::vector<const Team*> table;
    table.push_back(&playerTeam);
    for (const auto& opp : opponents) {
        table.push_back(&opp);
    }

    std::sort(table.begin(), table.end(), [](const Team* a, const Team* b) {
        if (a->getPoints() != b->getPoints()) {
            return a->getPoints() > b->getPoints();
        }
        return a->getGoalDifference() > b->getGoalDifference();
    });

    os << "=== League Standings ===\n";
    for (std::size_t i = 0; i < table.size(); ++i) {
        os << "  " << (i + 1) << ". " << table[i]->getName()
           << " - " << table[i]->getPoints() << " pts"
           << " (GD: " << table[i]->getGoalDifference() << ")\n";
    }
}

const Team& League::getPlayerTeam() const {
    return playerTeam;
}

const std::string& League::getLastReport() const {
    return lastReport;
}

double League::getTrainingCost() const {
    return trainingCost;
}

std::size_t League::getMarketSize() const {
    return transferMarket.size();
}

std::ostream& operator<<(std::ostream& os, const League& league) {
    os << "=== Mini Football Manager (Round " << league.currentRound
       << "/" << league.opponents.size() << ") ===\n";
    os << league.playerTeam;
    return os;
}