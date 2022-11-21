#ifndef PLAYER_H_
#define PLAYER_H_

#include "wet1util.h"
#include "AVLTree.h"

class Player
{
    Player(int playerId, int teamId, int gamesPlayed, int goals,
           int cards, bool goalKeeper);

    // playerId should be unique, so maybe we shouldn't allow copy?
    Player(const Player &player) = delete;
    ~Player() = default;
    StatusType update_player_stats(int playerId, int gamesPlayed,
                                   int scoredGoals, int cardsReceived);

private:
    int playerId;
    // TODO: is it necessary to hold the teamId here?
    int teamId;
    int gamesPlayed;
    int goals;
    int cards;
    bool goalKeeper;
};

#endif // PLAYER_H_