#include "worldcup23a1.h"
#include "Team.h"
#include "Player.h"
#include "Exception.h"
#include <cmath>

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
    // TODO: we don't need that, we have it in constructor
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
        playersByStats.insert(Tuple(goals, cards, playerId), newPlayer);

        //TODO: add player to the team and all the closest stuff
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
        removePlayerAux(playerId);
        return StatusType::SUCCESS;
    } catch(const KeyNotFound& error) {
        return StatusType::INVALID_INPUT;
    } catch(const bad_alloc& error) {
        return StatusType::ALLOCATION_ERROR;
    }
}

void world_cup_t::addPlayerAux(int playerId, int teamId, int gamesPlayed,
                                  int goals, int cards, bool goalKeeper) {
    
}

Player* world_cup_t::removePlayerAux(int playerId) {
    Player* removedPlayer = playersByID.remove(playerId);
    playersByStats.remove(removedPlayer->getStatsTuple());
    Player* rankNext = removedPlayer->getRankNext();
    Player* rankPrev= removedPlayer->getRankNext();
    
    rankNext->updatePrevRank(rankPrev);
    rankPrev->updateNextRank(rankNext);

    removedPlayer->removeFromTeam();
    
    if(topScorer->getId() == removedPlayer->getId()) {
        topScorer = removedPlayer->getRankPrev();
    }
    
    return removedPlayer;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
											int scoredGoals, int cardsReceived)
{
    if(playerId <= 0 || gamesPlayed < 0 || scoredGoals < 0 || cardsReceived < 0) {
        return StatusType::INVALID_INPUT;
    }
    
    try {
        Player* player = removePlayerAux(playerId);
        
        bool isGoalKeeper = player->isGoalKeeper();
        Team* team = player->getTeam();
        int newGamesPlayed = player->getNumPlayedGames() + team->getTotalGamesPlayed() + gamesPlayed;
        int newScoredGoals = player->getGoals() + scoredGoals;
        int newCards = player->getCards() + cardsReceived;
        
        addPlayerAux(playerId, team->getId(), newGamesPlayed, newScoredGoals, newCards, isGoalKeeper);
        
        return StatusType::SUCCESS;
    } catch(const KeyNotFound& error) {
        return StatusType::FAILURE;
    } catch(const bad_alloc& error) {
        return StatusType::ALLOCATION_ERROR;
    }
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
    if(playerId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    
    try {
        Player* player = playersByID.search(playerId);
        return output_t<int>(player->getNumPlayedGames());
    } catch (const KeyNotFound& error) {
        return output_t<int>(StatusType::FAILURE);
    } catch (const bad_alloc& error) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    
    try {
        Team* team = teams.search(teamId);
        return output_t<int>(team->getTotalPoints());
    } catch(const KeyNotFound& error) {
        return output_t<int>(StatusType::FAILURE);
    } catch(const bad_alloc& error) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
    Team* team1 = teams.search(teamId1);
    Team* team2 = teams.search(teamId2);
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

        Pair<Tuple, Player*>* playersArray = new Pair<Tuple, Player*>[numPlayersOverall];
        playersByStats.inorderDataToArray(playersArray);
        for(int i = 0; i < numPlayersOverall; i++)
            output[i] = playersArray[i].getValue()->getId();
        delete[] playersArray;
        return StatusType::SUCCESS;
    }

    if(teamId > 0) {
        try {
            Team* checkTeam = teams.search(teamId);
            int numPlayersTeam = checkTeam->getNumPlayers();
            if(numPlayersTeam == 0)
                return StatusType::FAILURE;
            Pair<Tuple, Player*>* teamPlayersArray = new Pair<Tuple, Player*>[numPlayersTeam];
    
            checkTeam->createStatsArray(teamPlayersArray);
            for(int i = 0; i < numPlayersTeam; i++)
                output[i] = teamPlayersArray[i].getValue()->getId();
            delete[] teamPlayersArray;
            return StatusType::SUCCESS;
        }
        catch (const KeyNotFound& k1) {
            return StatusType::FAILURE;
        }
    }

	return StatusType::INVALID_INPUT;
}

Player* world_cup_t::findPlayerClosest(int playerId, int teamId) const {
    Team* team = teams.search(teamId);
    Player* player = team->findPlayerById(playerId);
    Player* prev = player->getRankPrev();
    Player* next = player->getRankNext();
    
    if(prev == nullptr || next == nullptr) {
        if(prev == nullptr) {
            return next;
        } else {
            return prev;
        }
    }
    
  
    Player* closest = closestAux(player->getGoals(), prev, prev->getGoals(),
                                 next, next->getGoals());
    if(closest != nullptr) {
        return closest;
    }
    
    closest = closestAux(player->getCards(), prev, prev->getCards(),
                        next, next->getCards());
    
    if(closest != nullptr) {
        return closest;
    }
    
    closest = closestAux(player->getId(), prev, prev->getId(),
                         next, next->getId());
    
    if(closest != nullptr) {
        return closest;
    }
    
    if(prev->getId() > next->getId()) {
        return prev;
    }
    
    return next;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId) {
    if(playerId <= 0 || teamId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    
    if(numPlayersOverall <= 1) {
        return output_t<int>(StatusType::FAILURE);
    }
    
    try {
        Player* closest = findPlayerClosest(playerId, teamId);
        return output_t<int>(closest->getId());
    }
    
    catch(const KeyNotFound& error) {
        return output_t<int>(StatusType::FAILURE);
    }
    
    catch(const bad_alloc& error) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}

Player* closestAux(int playerVal,  Player* prev, int prevVal, Player* next,
                   int nextVal) {
    if(abs(playerVal-prevVal) < abs(playerVal-nextVal)) {
        return prev;
    }
    
    if(abs(playerVal-prevVal) > abs(playerVal-nextVal)) {
        return next;
    }
    
    return nullptr;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}
