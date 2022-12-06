#include "Player.h"
#include "Team.h"
#include "Exception.h"

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
};

void Player::removeFromTeam() {
    team->removePlayer(this);
}

int Player::getId() const {
    return playerId;
}

int Player::getNumPlayedGames() const {
    return gamesPlayed+team->getTotalGamesPlayed();
}

void Player::updateNextRank(Player* next) {
    nextRank = next;
}

void Player::updatePrevRank(Player* prev) {
    prevRank = prev;
}

Tuple Player::getStatsTuple() {
    return Tuple(goals, cards, playerId);
}



