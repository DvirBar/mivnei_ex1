#ifndef PLAYER_H_
#define PLAYER_H_

#include "wet1util.h"
#include "AVLTree.h"
#include "Team.h"
#include "Tuple.h"

// Forward declaration to break circular dependancy
class Team;

class Player {
public:
    Player(int playerId, Team* team, int gamesPlayed, int goals,
           int cards, bool goalKeeper);
    
    // playerId should be unique, so maybe we shouldn't allow copy?
    Player(const Player &player) = delete;
    ~Player() = default;
    StatusType update_stats(int gamesPlayed, int scoredGoals,
                            int cardsReceived);
    int getId() const;
    // TODO: implement
    int getNumPlayedGames() const;
    int getGoals() const;
    int getCards() const;
    Team* getTeam() const;
    bool isGoalKeeper() const;
    //
    Player* getRankNext() const;
    Player* getRankPrev() const;
    void updateNextRank(Player* next);
    void updatePrevRank(Player* prev);
    void removeFromTeam();
    Tuple getStatsTuple();

private:
    int playerId;
    int gamesPlayed;
    int goals;
    int cards;
    bool goalKeeper;
    Team* team;
    Player* nextRank;
    Player* prevRank;
};

#endif // PLAYER_H_
