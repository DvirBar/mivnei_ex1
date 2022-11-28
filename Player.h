#ifndef PLAYER_H_
#define PLAYER_H_

#include "wet1util.h"
#include "AVLTree.h"
#include "Team.h"

// Forward declaration to break circular dependancy
class Team;

class Player {
public:
    Player(int playerId, int teamId, int gamesPlayed, int goals,
           int cards, bool goalKeeper);

    // playerId should be unique, so maybe we shouldn't allow copy?
    Player(const Player &player) = delete;
    ~Player() = default;
    StatusType remove();
    StatusType update_stats(int gamesPlayed, int scoredGoals,
                                   int cardsReceived);
    output_t<int> get_num_played_games();

private:
    int playerId;
    int gamesPlayed;
    int goals;
    int cards;
    bool goalKeeper;
    Team* team;
    int closestPlayerId;
};

#endif // PLAYER_H_
