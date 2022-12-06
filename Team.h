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

    StatusType remove_team();
    int get_top_scorer();
    void addPoints(int pointsToAdd);
    void addGames(int numGamesToAdd);
    int getNumPlayers () const;
    int getNumGoalKeepers() const;
    bool isValidTeam() const;
    int getTotalGoals() const;
    int getTotalCards() const;
    int getTotalPoints() const;
    int getTotalGamesPlayed() const;
    // TODO: implement
    int getId() const;
    const AVLTree<Tuple, Player*>& getStatsTree() const;
    StatusType get_all_players(int* const output);
    void conclude_game(GameResult result);
    void addPlayer(Player* player);
    void removePlayer(Player* player);
    
    void createStatsArray(Pair<Tuple, Player*>* arr);
    void createIdsArray(Pair<int, Player*>* arr);
    
    // TODO: implement
    void fillStatsFromArray(Pair<Tuple, Player*>* arr, int size);
    void fillIdsFromArray(Pair<int, Player*>* arr, int size);

    Player* findPlayerById(int playerId);
    static Team* unite_teams(Team* team1, Team* team2, int newTeamId);
    // TODO: implement
    static void mergeArrays(Player** newArr, int newArrSize, Player** arr1,
                            int arr1Size, Player** arr2, int arr2Size);
    
    class TeamNotFound: public exception{};
private:
    int teamId;
    int totalPoints;
    int totalCards;
    int totalGoals;
    int numPlayers;
    int numGoalkeepers;
    int teamTopScorer;
    int totalGamesPlayed;
    AVLTree<int, Player*> teamPlayersByID;
    AVLTree<Tuple, Player*> teamPlayersByStats;
    
    // TODO: implement
    void clearTeam();
};

#endif // TEAM_H_
