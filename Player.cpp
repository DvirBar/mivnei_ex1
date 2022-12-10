#include "Player.h"
#include "Team.h"
#include "Exception.h"
#include "Tuple.h"

Player::Player(
    int playerId, Team* team, int gamesPlayed, int goals,
    int cards, bool goalKeeper):
    goalKeeper(goalKeeper),
    team(team),
    prevRank(nullptr),
    nextRank(nullptr)
{
    if (playerId <= 0 || gamesPlayed < 0 || goals < 0 ||
        cards < 0 || (gamesPlayed == 0 && (goals > 0 || cards > 0))) {
        throw InvalidArgumets();
    }
    
    this->playerId = playerId;
    this->gamesPlayed = gamesPlayed-team->getTotalGamesPlayed();
    this->goals = goals;
    this->cards = cards;
}

void Player::removeFromTeam() {
    team->removePlayer(this->getId());
}

int Player::getId() const {
    return playerId;
}

int Player::getNumPlayedGames() const {
    return gamesPlayed+team->getTotalGamesPlayed();
}

void Player::updateNextInRank(Player* next) {
    nextRank = next;
}

void Player::updatePrevInRank(Player* prev) {
    prevRank = prev;
}

const Tuple Player::getStatsTuple() const {
    return Tuple(goals, cards, playerId);
}

int Player::getCards() const {
    return cards;
}

int Player::getGoals() const {
    return goals;
}

bool Player::isGoalKeeper() const {
    return goalKeeper;
}

void Player::setGoals(int goals) {
    this->goals = goals;
}

void Player::setCards(int cards) {
    this->cards = cards;
}

void Player::setIsGoalKeeper(bool isGoalKeeper) {
    this->goalKeeper = isGoalKeeper;
}


void Player::setGamesPlayed(int gamesPlayed) {
    this->gamesPlayed = gamesPlayed;
}

Player* Player::getRankNext() const {
    return nextRank;
}

Player* Player::getRankPrev() const {
    return prevRank;
}

Team* Player::getTeam() const {
    return team;
}

void Player::setTeam(Team* team) {
    this->team = team;
}

