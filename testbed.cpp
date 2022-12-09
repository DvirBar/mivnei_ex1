#include "AVLTree.h"
#include "Player.h"
#include "Exception.h"
#include "worldcup23a1.h"
#include <iostream>

using namespace std;


int main() {
    world_cup_t *obj = new world_cup_t();
    int playerId = 1;
    StatusType res;
    for (int teamId = 1; teamId < 10; teamId += 2)
    {
        res = obj->add_team(teamId, 1000 / ((6 - teamId) * (6 - teamId)));

        for (int startingId = playerId; playerId < startingId + 10 + teamId; ++playerId)
        {
            res = obj->add_player(playerId, teamId, 1, playerId, 3, true);
        }
        ++playerId;
    }

    res = obj->add_team(4, 1000000);

    output_t<int> res1 = obj->knockout_winner(8, 9);
    cout << res1.ans();
    delete obj;
}
