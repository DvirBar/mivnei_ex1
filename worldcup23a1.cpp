#include "worldcup23a1.h"
#include "Team.h"
#include "Player.h"
#include "Exception.h"

world_cup_t::world_cup_t()
{
	// TODO: Your code goes here
}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}

StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId <= 0 || points < 0)
        return StatusType::INVALID_INPUT;

    if(!teams.isExist(teamId)) {
        return StatusType::FAILURE;
    }

    try {
        teams.insert(teamId, new Team(teamId, points));
    }
    catch (const bad_alloc& badAlloc) {
        return StatusType::ALLOCATION_ERROR;
    }

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if(teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        Team* team = teams.search(teamId);
        if(team->getNumPlayers() > 0) {
            return StatusType::FAILURE;
        }
        teams.remove(teamId);
    } catch(const KeyNotFound& error) {
        return StatusType::FAILURE;
    } catch(const bad_alloc& error) {
        return StatusType::ALLOCATION_ERROR;
    }
    
	return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
								   int goals, int cards, bool goalKeeper)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
											int scoredGoals, int cardsReceived)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if((teamId1 == teamId2) || (teamId1 <= 0) || (teamId2 <= 0)) {
        return StatusType::INVALID_INPUT;
    }

    Team* checkTeam1 = teams.search(teamId1);
    Team* checkTeam2 = teams.search(teamId2);

    if((checkTeam1 == nullptr) || (checkTeam2 == nullptr))
        return StatusType::FAILURE;

    if((!checkTeam1->isValidTeam()) || (!checkTeam2->isValidTeam()))
        return StatusType::FAILURE;

    int totalScoreTeam1 = checkTeam1->getTotalPoints() + checkTeam1->getTotalGoals() - checkTeam1->getTotalCards();
    int totalScoreTeam2 = checkTeam2->getTotalPoints() + checkTeam2->getTotalGoals() - checkTeam2->getTotalCards();

    if(totalScoreTeam1 > totalScoreTeam2) {
        checkTeam1->addPoints(3);
    }
    else if(totalScoreTeam1 < totalScoreTeam2) {
        checkTeam2->addPoints(3);
    }
    else {
        checkTeam1->addPoints(1);
        checkTeam2->addPoints(1);
    }

    checkTeam1->addGames(1);
    checkTeam2->addGames(1);

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	// TODO: Your code goes here
	return 22;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
	return 30003;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    if(teamId > 0) {
        Team* checkTeam = teams.search(teamId);
        if(checkTeam == nullptr) {
            output_t<int> noTeam = output_t<int>(StatusType::FAILURE);
            return noTeam;
        }
        else {
            output_t<int> successTeam = checkTeam->get_top_scorer();
            return successTeam;
        }
    }

    if(teamId < 0) {
        if(playersByID.isEmpty()) {
            output_t<int> noPlayers = output_t<int>(StatusType::FAILURE);
            return noPlayers;
        }
        else {
            output_t<int> successPlayer = output_t<int>(topScorer);
            return successPlayer;
        }
    }
    
    output_t<int> invalidInput = output_t<int>(StatusType::INVALID_INPUT);
    return invalidInput;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    if(teamId < 0) {
        output_t<int> playersOverall = output_t<int>(numPlayersOverall);
        return playersOverall;
    }

    if(teamId > 0) {
        try {
            Team* checkTeam = teams.search(teamId);
            output_t<int> playersInTeam = output_t<int>(checkTeam->getNumPlayers());
            return playersInTeam;
        }
        catch (const KeyNotFound& k1) {
            output_t<int> teamNotFound = output_t<int>(StatusType::FAILURE);
            return teamNotFound;
        }
    }

    output_t<int> invalidInput = output_t<int>(StatusType::INVALID_INPUT);
    return invalidInput;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
    if(output == nullptr) {
        return StatusType::INVALID_INPUT;
    }
    if(teamId < 0) {
        if(numPlayersOverall == 0) {
            return StatusType::FAILURE;
        }

        Player** playersArray = new Player*[numPlayersOverall];
        playersByStats.inorderDataToArray(playersArray);
        for(int i = 0; i < numPlayersOverall; i++)
            output[i] = playersArray[i]->getID();
        delete[] playersArray;
        return StatusType::SUCCESS;
    }

    if(teamId > 0) {
        try {
            Team* checkTeam = teams.search(teamId);
            int numPlayersTeam = checkTeam->getNumPlayers();
            if(numPlayersTeam == 0)
                return StatusType::FAILURE;
            Player** teamPlayersArray = new Player*[numPlayersTeam];

            AVLTree<Tuple, Player*> teamPlayerTree = checkTeam->getStatsTree();
            teamPlayerTree.inorderDataToArray(teamPlayersArray);

            for(int i = 0; i < numPlayersTeam; i++)
                output[i] = teamPlayersArray[i]->getID();
            delete[] teamPlayersArray;
            return StatusType::SUCCESS;
        }
        catch (const KeyNotFound& k1) {
            return StatusType::FAILURE;
        }
    }

	return StatusType::INVALID_INPUT;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
	return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}
