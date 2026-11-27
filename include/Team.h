#pragma once

#include "Footballer.h"
#include "Squad.h"
#include <iostream>
#include <string>

class Team {
private:
    std::string name;
    Squad squad;
    double budget;
    int points;
    int goalsFor;
    int goalsAgainst;

public:
    Team(std::string name, double startingBudget);

    const std::string& getName() const;
    double getBudget() const;
    int getPoints() const;
    int getGoalDifference() const;
    const Squad& getSquad() const;

    void addInitialPlayer(const Footballer& player);
    bool buyPlayer(const Footballer& player, int maxSquadSize);
    bool trainTeam(double cost);
    void restTeam();
    void applyMatchResult(int scored, int conceded, double reward);

    friend std::ostream& operator<<(std::ostream& os, const Team& team);
};