#include "Team.h"
#include "AVLTree.h"

Team::Team(int teamId, int points)
{
    if (teamId <= 0 || points < 0)
    {
        // TODO: throw an error
        return;
    }

    this->teamId = teamId;
    this->points = points;
    // TOOD: init teamPlayers
}