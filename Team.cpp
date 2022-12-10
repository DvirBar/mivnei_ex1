#include "Team.h"
#include "AVLTree.h"
#include "Tuple.h"
#include "Pair.h"

Team::Team(int teamId, int points) :
    teamId(teamId),
    totalPoints(points),
    totalCards(0),
    totalGoals(0),
    numGoalkeepers(0),
    teamTopScorer(nullptr),
    nextValidRank(nullptr),
    prevValidRank(nullptr),
    totalGamesPlayed(0),
    teamPlayersByID(),
    teamPlayersByStats()
{}

int Team::getNumPlayers() const {
    return teamPlayersByID.getNumNodes();
}

int Team::getNumGoalKeepers() const {
    return numGoalkeepers;
}

bool Team::isValidTeam() const {
    return ((getNumPlayers() >= 11) && (numGoalkeepers > 0));
}

void Team::addPoints(int pointsToAdd) {
    totalPoints += pointsToAdd;
}

void Team::addGame() {
    totalGamesPlayed++;
}

void Team::setGoalGoalKeepers(int numGoalkeeper) {
    this->numGoalkeepers = numGoalkeeper;
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

int Team::getTotalGamesPlayed() const {
    return totalGamesPlayed;
}

const AVLTree<Tuple, Player *>& Team::getStatsTree() const {
    return teamPlayersByStats;
}

void Team::uniteTopScorers(Team* team1, Team* team2) {
    if(team1->isEmpty() && team2->isEmpty()) {
        return;
    }
    
    if(team1->isEmpty()) {
        teamTopScorer = team2->getTopScorer();
        return;
    }
    
    if(team2->isEmpty()) {
        teamTopScorer = team1->getTopScorer();
        return;
    }
    
    if(team1->getTopScorer()->getStatsTuple() > team2->getTopScorer()->getStatsTuple()) {
        setTopScorer(team1->getTopScorer());
    }
    
    else {
        setTopScorer(team2->getTopScorer());
    }
}

Team* Team::unite_teams(Team* team1, Team* team2, int newTeamId) {
    Team* newTeam = new Team(newTeamId, team1->getTotalPoints() + team2->getTotalPoints());
    
    // TODO: update stats
    newTeam->setCards(team1->getTotalCards()+team2->getTotalCards());
    newTeam->setGoals(team1->getTotalGoals()+team2->getTotalGoals());
    newTeam->setGoalGoalKeepers(team1->getNumGoalKeepers()+team2->getNumGoalKeepers());
    newTeam->uniteTopScorers(team1, team2);
    
    int team1Size = team1->getNumPlayers();
    int team2Size = team2->getNumPlayers();
    int newArrSize = team1Size + team2Size;
    
    if(newArrSize == 0) {
        return newTeam;
    }
        
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
    
    for(int i=0; i<newArrSize; i++) {
        Player* curPlayer = newIdsArr[i].getValue();
        curPlayer->setGamesPlayed(curPlayer->getNumPlayedGames());
        curPlayer->setTeam(newTeam);
    }
    
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

void Team::removePlayer(int playerId) {
    Player* playerToRemove = teamPlayersByID.remove(playerId);
    teamPlayersByStats.remove(playerToRemove->getStatsTuple());
    int playerGoals = playerToRemove->getGoals();
    int playerCards = playerToRemove->getCards();
    setGoals(-playerGoals);
    setCards(-playerCards);

    if(playerToRemove->isGoalKeeper())
        numGoalkeepers--;
}

bool Team::isEmpty() {
    return teamPlayersByID.isEmpty();
}

void Team::addPlayer(Player* playerToInsert) {
    teamPlayersByID.insert(playerToInsert->getId(), playerToInsert);
    teamPlayersByStats.insert(playerToInsert->getStatsTuple(), playerToInsert);
    setGoals(playerToInsert->getGoals());
    setCards(playerToInsert->getCards());
    if(playerToInsert->isGoalKeeper())
        numGoalkeepers++;
}

int Team::getId() const{
    return teamId;
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

void Team::updateTopScorer(Player *player) {
    if(teamTopScorer == nullptr ||
       teamTopScorer->getStatsTuple() < player->getStatsTuple())
        teamTopScorer = player;
}
