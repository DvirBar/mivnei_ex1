#include "Team.h"
#include "AVLTree.h"
#include "Tuple.h"
#include "Pair.h"

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

int Team::getTotalGamesPlayed() const {
    return totalGamesPlayed;
}

const AVLTree<Tuple, Player *>& Team::getStatsTree() const {
    return teamPlayersByStats;
}


void Team::removePlayer(Player* player) {
    teamPlayersByID.remove(player->getId());
    teamPlayersByStats.remove(player->getStatsTuple());
    numPlayers--;
}

Team* Team::unite_teams(Team* team1, Team* team2, int newTeamId) {
    int team1Size = team1->getNumPlayers();
    int team2Size = team2->getNumPlayers();
    int newArrSize = team1Size + team2Size;
    Team* newTeam = new Team(newTeamId, newArrSize);
    
    Pair<Tuple, Player*>* newStatsArr = new Pair<Tuple, Player*>[newArrSize];
    Pair<Tuple, Player*>* statsArr1 = new Pair<Tuple, Player*>[team1->getNumPlayers()];
    Pair<Tuple, Player*>* statsArr2 = new Pair<Tuple, Player*>[team2->getNumPlayers()];
    Pair<int, Player*>* newIdsArr = new Pair<int, Player*>[newArrSize];
    Pair<int, Player*>* idsArr1 = new Pair<int, Player*>[team1->getNumPlayers()];
    Pair<int, Player*>* idsArr2 = new Pair<int, Player*>[team2->getNumPlayers()];
    
    team1->createStatsArray(statsArr1);
    team2->createStatsArray(statsArr2);
    team1->createIdsArray(idsArr1);
    team2->createIdsArray(idsArr2);
    
    AVLTree<Tuple, Player*>::mergeArrays(newStatsArr, newArrSize, statsArr1, team1Size, statsArr2, team2Size);
    AVLTree<int, Player*>::mergeArrays(newIdsArr, newArrSize, idsArr1, team1Size, idsArr2, team2Size);
    
    newTeam->fillStatsFromArray(newStatsArr, newArrSize);
    newTeam->fillIdsFromArray(newIdsArr, newArrSize);
    
    delete[] newStatsArr;
    delete[] statsArr1;
    delete[] statsArr2;
    delete[] newIdsArr;
    delete[] idsArr1;
    delete[] idsArr2;
    
    return newTeam;
}

void Team::createStatsArray(Pair<Tuple, Player*>* arr) {
    teamPlayersByStats.inorderDataToArray(arr);
}

void Team::createIdsArray(Pair<int, Player*>* arr) {
    teamPlayersByID.inorderDataToArray(arr);
}

void Team::fillStatsFromArray(Pair<Tuple, Player*>* arr, int size) {
    teamPlayersByStats.populateFromArray(arr, size);
}

void Team::fillIdsFromArray(Pair<int, Player*>* arr, int size) {
    teamPlayersByID.populateFromArray(arr, size);
}

Player* Team::findPlayerById(int playerId) {
    return teamPlayersByID.search(playerId);
}
