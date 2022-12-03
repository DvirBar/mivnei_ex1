#include "worldcup23a1.h"
#include "Team.h"
#include "Player.h"
#include "Exception.h"
#include "LinkedList.h"

world_cup_t::world_cup_t():
teams(),
playersByStats(),
playersByID(),
validKnockoutTeams(),
topScorer(nullptr),
numPlayersOverall(0)
{}

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
    if(playerId <= 0 || teamId <= 0 || gamesPlayed < 0 || goals < 0 || cards < 0)
        return StatusType::INVALID_INPUT;
    if(gamesPlayed == 0 && (cards > 0 || goals > 0))
        return StatusType::INVALID_INPUT;
    try {
        if(playersByID.isExist(playerId))
            return StatusType::FAILURE;

        Team* checkTeam = teams.search(teamId);
        Player* newPlayer = new Player(playerId, checkTeam, gamesPlayed, goals, cards, goalKeeper);

        playersByID.insert(playerId, newPlayer);
        playersByStats.insert(newPlayer->getStatsTuple(), newPlayer);
        checkTeam->addPlayer(newPlayer);
        numPlayersOverall++;

        if(checkTeam->isValidTeam()) {
            validKnockoutTeams.insert(teamId,checkTeam);
            checkTeam->setNextValidRank(validKnockoutTeams.nextInorder(teamId));
            checkTeam->setPrevValidRank(validKnockoutTeams.prevInorder(teamId));
        }

        newPlayer->updatePrevInRank(playersByStats.prevInorder(newPlayer->getStatsTuple()));
        newPlayer->updateNextInRank(playersByStats.nextInorder(newPlayer->getStatsTuple()));

        if(newPlayer->getStatsTuple() > topScorer->getStatsTuple()) {
            topScorer = newPlayer;
        }
        if(newPlayer->getStatsTuple() > checkTeam->getTopScorer()->getStatsTuple())
            checkTeam->setTopScorer(newPlayer);
    }

    catch(const KeyNotFound& keyNotFound) {
        return StatusType::FAILURE;
    }
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
    if(playerId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    
    try {
        Player* removedPlayer = playersByID.remove(playerId);
        playersByStats.remove(removedPlayer->getStatsTuple());
        Player* lastClosest = playersByID.search(removedPlayer->getClosestId());
        Player* lastRefBy = playersByID.search(removedPlayer->getRefById());
        
        lastClosest->updateClosest(findPlayerClosest(lastClosest->getStatsTuple()));
        lastRefBy->updateClosest(findPlayerClosest(lastRefBy->getStatsTuple()));

        removedPlayer->removeFromTeam();
    } catch(const KeyNotFound& error) {
        return StatusType::INVALID_INPUT;
    } catch(const bad_alloc& error) {
        return StatusType::ALLOCATION_ERROR;
    }
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
            output_t<int> successTeam = checkTeam->getTopScorer()->getId();
            return successTeam;
        }
    }

    if(teamId < 0) {
        if(playersByID.isEmpty()) {
            output_t<int> noPlayers = output_t<int>(StatusType::FAILURE);
            return noPlayers;
        }
        else {
            output_t<int> successPlayer = output_t<int>(topScorer->getId());
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
        catch (const KeyNotFound& keyNotFound) {
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
            output[i] = playersArray[i]->getId();
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
            //TODO: make Team Method that does this, no need for team tree access.
            const AVLTree<Tuple, Player*>& teamPlayerTree = checkTeam->getStatsTree();
            teamPlayerTree.inorderDataToArray(teamPlayersArray);

            for(int i = 0; i < numPlayersTeam; i++)
                output[i] = teamPlayersArray[i]->getId();
            delete[] teamPlayersArray;
            return StatusType::SUCCESS;
        }
        catch (const KeyNotFound& keyNotFound) {
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
    if(minTeamId < 0 || maxTeamId < 0 || maxTeamId < minTeamId)
        return StatusType::INVALID_INPUT;

    Team* firstTeamInRange;

    try{
        firstTeamInRange = validKnockoutTeams.findFirstInRange(minTeamId);
    }
    catch (const KeyNotFound& keyNotFound) {
        return StatusType::INVALID_INPUT;
    }

//    LinkedList<int, int>* teamList = new LinkedList<int,int>(0,0);
//    Team* teamPointer = firstTeamInRange;
//    while(teamPointer->getTeamId() <= maxTeamId) {
//        try {
//            teamList->insert(teamPointer->getTeamId(), teamPointer->getTotalStats());
//        }
//        catch (const bad_alloc& badAlloc) {
//            return StatusType::ALLOCATION_ERROR;
//        }
//        teamPointer = teamPointer->getNextValidRank();
//    }

//    LinkedList<int,int>* currentNode = teamList->


}
