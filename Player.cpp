#include "Player.h"
#include "Team.h"

Player::Player(
    int playerId, int teamId, int gamesPlayed, int goals,
    int cards, bool goalKeeper) : goalKeeper(goalKeeper)
{
    if (playerId <= 0 || teamId <= 0 || gamesPlayed < 0 || goals < 0 ||
        cards < 0 || (gamesPlayed == 0 && (goals > 0 || cards > 0)))
    {
        // TODO: Throw an error
        return;
    }
    
    // TODO: find team and add it

    this->playerId = playerId;
    this->gamesPlayed = gamesPlayed;
    this->goals = goals;
    this->cards = cards;
};


