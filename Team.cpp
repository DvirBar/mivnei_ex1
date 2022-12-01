#include "Team.h"
#include "AVLTree.h"
#include "Tuple.h"

Team::Team(int teamId, int points)
{
    if (teamId <= 0 || points < 0)
    {
        // TODO: throw an error
        return;
    }

    this->teamId = teamId;
    this->totalPoints = points;
    // TOOD: init teamPlayers
}

int Team::getNumPlayers() const {
    return numPlayers;
}

int Team::getNumGoalKeepers() const {
    return numGoalkeepers;
}

bool Team::isValidTeam() const {
    return ((numPlayers >= 11) && (numGoalkeepers > 0));
}

void Team::addPoints(int pointsToAdd) {
    totalPoints += pointsToAdd;
}

void Team::addGames(int numGamesToAdd) {
    totalGamesPlayed += numGamesToAdd;
}

int Team::getTotalGoals() const {
    return totalGoals;
}

int Team::getTotalCards() const {
    return totalCards;
}

int Team::getTotalPoints() const {
    return totalPoints;
}

const AVLTree<Tuple, Player *>& Team::getStatsTree() const {
    return teamPlayersByStats;
}


void Team::removePlayer(int playerId) {
    teamPlayersByID.remove(playerId);
    teamPlayersByID.remove(playerId);
}

