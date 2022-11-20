#ifndef TEAM_H_
#define TEAM_H_

#include <memory>
#include "wet1util.h"
#include "BinaryTree.h"
#include "Player.h"

using namespace std;

class Team
{
    Team(int teamId, int points);
    Team(const Team &team);
    ~Team();

    StatusType add_team(int teamId, int points);
    StatusType remove_team(int teamId);
    output_t<int> get_team_points(int teamId);

private:
    int teamId;
    int points;
    // TODO: Might need to update everything all at once
    // int totalGamesPlayed;
    BinaryTree<shared_ptr<Player>> teamPlayers;
};

#endif // TEAM_H_