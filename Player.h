#ifndef PLAYER_H_
#define PLAYER_H_

#include "wet1util.h"
#include "AVLTree.h"
#include "Team.h"

// Forward declaration to break circular dependancy
class Team;

class Player {
    Player(int playerId, Team* team, int gamesPlayed, int goals,
           int cards, bool goalKeeper);

    // playerId should be unique, so maybe we shouldn't allow copy?
    Player(const Player &player) = delete;
    ~Player() = default;
    StatusType remove();
    StatusType update_stats(int gamesPlayed, int scoredGoals,
                                   int cardsReceived);
    output_t<int> get_num_played_games();

private:
    static const int MAX_CLOSEST = 2;
    int playerId;
    int gamesPlayed;
    int goals;
    int cards;
    bool goalKeeper;
    Team* team;
    int closestPlayerId;
    int refByClosest[MAX_CLOSEST];
};

#endif // PLAYER_H_
