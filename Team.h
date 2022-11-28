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
    Team(int teamId, int points);
    Team(const Team &team);
    ~Team();

    StatusType add_team(int teamId, int points);
    StatusType remove_team(int teamId);
    output_t<int> get_team_points(int teamId);
    int get_top_scorer();
    int get_all_players_count();
    StatusType get_all_players(int* const output);
    output_t<int> get_closest_player(int playerId, int teamId);
    void conclude_game(GameResult result);
    static StatusType unite_teams(int teamId1, int teamId2, int newTeamId);

private:
    int teamId;
    int points;
    int num_players;
    int num_goalkeepers;
    // int totalGamesPlayed;
    AVLTree<int, Player*> team_players;
};

#endif // TEAM_H_
