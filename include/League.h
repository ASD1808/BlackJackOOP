#pragma once

#include "Footballer.h"
#include "Team.h"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

class League {
private:
    Team playerTeam;
    std::vector<Team> opponents;
    std::vector<Footballer> transferMarket;
    double winReward;
    double drawReward;
    double trainingCost;
    int maxSquadSize;
    std::size_t currentRound;
    std::string lastReport;

    void loadLeague(const std::string& leaguePath);
    void loadMarket(const std::string& marketPath);
    std::string simulateMatch(Team& home, Team& away);

public:
    explicit League(const std::string& leaguePath = "assets/league.json",
                    const std::string& marketPath = "assets/market.json");

    bool hasNextMatch() const;
    bool playNextMatch();
    bool buyFromMarket(std::size_t marketIndex);
    bool trainPlayerTeam();
    void restPlayerTeam();

    void displayMarket(std::ostream& os) const;
    void displayStandings(std::ostream& os) const;

    const Team& getPlayerTeam() const;
    const std::string& getLastReport() const;
    double getTrainingCost() const;
    std::size_t getMarketSize() const;

    friend std::ostream& operator<<(std::ostream& os, const League& league);
};