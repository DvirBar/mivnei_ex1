#ifndef TEAM_H_
#define TEAM_H_

#include <exception>
#include "wet1util.h"
#include "AVLTree.h"
#include "Player.h"
#include "Tuple.h"

using namespace std;

enum struct GameResult
{
    WON = 0,
    LOST = 1,
    TIE = 2
};

// Forward declaration to break circular dependancy
class Player;

class Team {
public:
    Team(int teamId, int points);
    Team(const Team &team);
    ~Team();

//    StatusType remove_team();
    void addPoints(int pointsToAdd);
    void addGames(int numGamesToAdd);
    void setCards(int cards);
    void setGoals(int goals);
    int getNumPlayers () const;
    int getNumGoalKeepers() const;
    bool isValidTeam() const;
    int getTotalGoals() const;
    int getTotalCards() const;
    int getTotalPoints() const;
    Team* getNextValidRank() const;
    Team* getPrevValidRank() const;
    int getTeamId() const;
    const AVLTree<Tuple, Player*>& getStatsTree() const;
    int getTotalStats() const;
    //    StatusType get_all_players(int* const output);
//    void conclude_game(GameResult result);
//    static StatusType unite_teams(int teamId1, int teamId2, int newTeamId);
    void removePlayer(int playerId);
    void addPlayer(Player* playerToInsert);
    Player* getTopScorer() const;
    void setTopScorer(Player* newTopScorer);
    void setNextValidRank(Team* next);
    void setPrevValidRank(Team* prev);

    class TeamNotFound: public exception{};
private:
    int teamId;
    int totalPoints;
    int totalCards;
    int totalGoals;
    int numPlayers;
    int numGoalkeepers;
    Player* teamTopScorer;
    Team* nextValidRank;
    Team* prevValidRank;
    int totalGamesPlayed;
    AVLTree<int, Player*> teamPlayersByID;
    AVLTree<Tuple, Player*> teamPlayersByStats;
};

#endif // TEAM_H_
