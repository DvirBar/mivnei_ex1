#include "Team.h"
#include "AVLTree.h"
#include "Tuple.h"

Team::Team(int teamId, int points) :
teamId(teamId),
totalPoints(points),
totalCards(0),
totalGoals(0),
numPlayers(0),
numGoalkeepers(0),
teamTopScorer(nullptr),
nextValidRank(nullptr),
prevValidRank(nullptr),
totalGamesPlayed(0),
teamPlayersByID(),
teamPlayersByStats()
{}

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

void Team::setCards(int cards) {
    totalCards += cards;
}

void Team::setGoals(int goals) {
    totalGoals += goals;
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
    Player* playerToRemove = teamPlayersByID.remove(playerId);
    teamPlayersByStats.remove(playerToRemove->getStatsTuple());
    int playerGoals = playerToRemove->getGoals();
    int playerCards = playerToRemove->getCards();
    setGoals(-playerGoals);
    setCards(-playerCards);

    numPlayers--;

    if(playerToRemove->isGoalKeeper())
        numGoalkeepers--;

}

void Team::addPlayer(Player* playerToInsert) {
    teamPlayersByID.insert(playerToInsert->getId(), playerToInsert);
    teamPlayersByStats.insert(playerToInsert->getStatsTuple(), playerToInsert);
    setGoals(playerToInsert->getGoals());
    setCards(playerToInsert->getCards());
    numPlayers++;
    if(playerToInsert->isGoalKeeper())
        numGoalkeepers++;
}

Player* Team::getTopScorer() const {
    return teamTopScorer;
}

void Team::setTopScorer(Player* newTopScorer) {
    teamTopScorer = newTopScorer;
}

void Team::setNextValidRank(Team* next) {
    nextValidRank = next;
}

void Team::setPrevValidRank(Team* prev) {
    prevValidRank = prev;
}

int Team::getTeamId() const {
    return teamId;
}

Team* Team::getNextValidRank() const {
    return nextValidRank;
}

Team* Team::getPrevValidRank() const {
    return prevValidRank;
}

int Team::getTotalStats() const {
    return totalPoints + totalGoals - totalCards;
}
