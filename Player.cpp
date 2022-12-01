#include "Player.h"
#include "Team.h"
#include "Exception.h"

Player::Player(
    int playerId, Team* team, int gamesPlayed, int goals,
    int cards, bool goalKeeper):
    goalKeeper(goalKeeper),
    team(team),
    closestPlayerId(0)
{
    if (playerId <= 0 || gamesPlayed < 0 || goals < 0 ||
        cards < 0 || (gamesPlayed == 0 && (goals > 0 || cards > 0))) {
        throw InvalidArgumets();
    }
    
    this->playerId = playerId;
    this->gamesPlayed = gamesPlayed;
    this->goals = goals;
    this->cards = cards;
};

void Player::removeFromTeam() {
    team->removePlayer(playerId);
}

int Player::getId() const {
    return playerId;
}


