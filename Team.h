#ifndef TEAM_H_
#define TEAM_H_

#include <memory>
#include "wet1util.h"
#include "AVLTree.h"
#include "Player.h"

using namespace std;

enum struct GameResult
{
    WON = 0,
    LOST = 1,
    TIE = 2
};

class Team {
public:
    Team(int teamId, int points);
    Team(const Team &team);
    ~Team();

    StatusType add_team(int teamId, int points);
    StatusType remove_team(int teamId);
    output_t<int> get_team_points(int teamId);
    int get_top_scorer();
    int getNumPlayers () const;
    int getNumGoalKeepers() const;
    bool isValidTeam() const;
    void addPoints(int pointsToAdd);
    void addGames(int numGamesToAdd);
    int getTotalGoals() const;
    int getTotalCards() const;
    int getTotalPoints() const;
    StatusType get_all_players(int* const output);
    output_t<int> get_closest_player(int playerId, int teamId);
    void conclude_game(GameResult result);
    static StatusType unite_teams(int teamId1, int teamId2, int newTeamId);

private:
    int teamId;
    int totalPoints;
    int totalCards;
    int totalGoals;
    int num_players;
    int num_goalkeepers;
    int teamtopScorer;
    int totalGamesPlayed;
    AVLTree<int, Player*> team_players;
};

#endif // TEAM_H_
