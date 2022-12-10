//#include "AVLTree.h"
//#include "Player.h"
//#include "Exception.h"
//#include "worldcup23a1.h"
//#include <iostream>
//
//using namespace std;
//
//
//int main() {
//    world_cup_t *obj = new world_cup_t();
//
//    obj->add_team(1, 10);
//    obj->add_team(2, 10);
//    obj->add_team(3, 5);
//    obj->add_team(4, 5);
//    // obj->add_player(<#int playerId#>, <#int teamId#>, <#int gamesPlayed#>, <#int goals#>, <#int cards#>, <#bool goalKeeper#>)
//    obj->add_player(200, 1, 3, 2, 0, true);
//    for(int i=1; i<11; i++) {
//        obj->add_player(i, 1, 1, 2, 0, false);
//    }
//
//    obj->add_player(201, 2, 1, 0, 0, true);
//    for(int i=0; i<11; i++) {
//        obj->add_player(i+20, 2, 1, 0, 0, false);
//    }
//
//    obj->add_player(202, 3, 1, 0, 0, true);
//    for(int i=0; i<11; i++) {
//        obj->add_player(i+3000, 3, i+2, 0, 0, false);
//    }
//
//
//    obj->play_match(1, 2);
//    obj->play_match(1, 2);
//    obj->play_match(1, 2);
//    obj->play_match(1, 2);
//    obj->play_match(1, 2);
//    obj->play_match(2, 3);
//    obj->play_match(2, 3);
//    obj->play_match(2, 3);
//    obj->play_match(2, 3);
////    cout << obj->get_num_played_games(1).ans() << endl;
//    cout << obj->get_num_played_games(1).ans() << endl;
////    obj->update_player_stats(1, 3, 0, 0);
////    cout << obj->get_num_played_games(1).ans() << endl;
////    cout << obj->get_num_played_games(20).ans() << endl;
//    obj->unite_teams(1, 2, 8);
//    cout << obj->get_num_played_games(1).ans() << endl;
////    cout << obj->get_num_played_games(20).ans() << endl;
//    obj->play_match(8, 3);
//    obj->play_match(8, 3);
//    obj->play_match(8, 3);
//    obj->unite_teams(1, 4, 8);
//    cout << obj->get_num_played_games(1).ans() << endl;
//
//}
