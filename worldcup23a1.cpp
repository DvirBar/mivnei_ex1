#include "worldcup23a1.h"
#include "Team.h"
#include "Player.h"
#include "Exception.h"
#include <cmath>
#include "Pair.h"

world_cup_t::world_cup_t():
    teams(),
    playersByStats(),
    playersByID(),
    validKnockoutTeams(),
    topScorer(nullptr),
    numPlayersOverall(0)
{}

world_cup_t::~world_cup_t() {
    Pair<int, Team*>* teamsArray = new Pair<int, Team*>[teams.getNumNodes()];
    Pair<int, Player*>* playersArray = new Pair<int, Player*>[playersByID.getNumNodes()];
    
    teams.inorderDataToArray(teamsArray);
    playersByID.inorderDataToArray(playersArray);
    
    for(int i=0; i<teams.getNumNodes(); i++) {
        delete teamsArray[i].getValue();
    }
    
    for(int i=0; i<playersByID.getNumNodes(); i++) {
        delete playersArray[i].getValue();
    }
}

StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId <= 0 || points < 0)
        return StatusType::INVALID_INPUT;

    if(teams.isExist(teamId)) {
        return StatusType::FAILURE;
    }

    try {
        Team* newTeam = new Team(teamId,points);
        teams.insert(teamId, newTeam);
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

void world_cup_t::updateOverallTopScorer(Player *player) {
    if((topScorer == nullptr) || (topScorer->getStatsTuple() < player->getStatsTuple()))
        topScorer = player;

}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
								   int goals, int cards, bool goalKeeper)
{
    // TODO: we don't need that, we have it in constructor
    if(playerId <= 0 || gamesPlayed < 0 || goals < 0 ||
       cards < 0 || (gamesPlayed == 0 && (goals > 0 || cards > 0))) {
        return StatusType::INVALID_INPUT;
    }

    try {
        if(playersByID.isExist(playerId))
            return StatusType::FAILURE;

        Team* checkTeam = teams.search(teamId);
        Player* newPlayer = new Player(playerId, checkTeam, gamesPlayed, goals, cards, goalKeeper);
        addPlayerAux(newPlayer, checkTeam);
    }

    catch(const KeyNotFound& keyNotFound) {
        return StatusType::FAILURE;
    }
    
    catch(const bad_alloc& bad_alloc) {
        return StatusType::ALLOCATION_ERROR;
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

void world_cup_t::addPlayerAux(Player* player, Team* team) {


    bool wasValid = team->isValidTeam();
    playersByID.insert(player->getId(), player);
    playersByStats.insert(player->getStatsTuple(), player);
    team->addPlayer(player);

    if(!wasValid && team->isValidTeam()) {
        validKnockoutTeams.insert(team->getId(), team);
        team->setNextValidRank(validKnockoutTeams.nextInorder(team->getId()));
        team->setPrevValidRank(validKnockoutTeams.prevInorder(team->getId()));
    }

    player->updatePrevInRank(playersByStats.prevInorder(player->getStatsTuple()));
    player->updateNextInRank(playersByStats.nextInorder(player->getStatsTuple()));

    updateOverallTopScorer(player);
    team->updateTopScorer(player);
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
        
        addPlayerAux(player, team);
        
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
    if(newTeamId <= 0 || teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }
    
    try {
        Team* team1 = teams.search(teamId1);
        Team* team2 = teams.search(teamId2);
        if(teams.isExist(newTeamId)) {
            return StatusType::FAILURE;
        }
        
        Team::unite_teams(team1, team2, newTeamId);
        
        return StatusType::SUCCESS;
    } catch (const KeyNotFound& error) {
        return StatusType::FAILURE;
    } catch (const bad_alloc& error) {
        return StatusType::ALLOCATION_ERROR;
    }
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
        catch (const KeyNotFound& keyNotFound) {
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
    if(minTeamId < 0 || maxTeamId < 0 || maxTeamId < minTeamId)
        return StatusType::INVALID_INPUT;

    Team* firstTeamInRange;

    try {
        firstTeamInRange = validKnockoutTeams.findFirstInRange(minTeamId);
        Team *currentTeam = firstTeamInRange;
        int initialNumTeams = 0;

        while (currentTeam != nullptr && currentTeam->getTeamId() < maxTeamId) {
            initialNumTeams++;
            currentTeam = currentTeam->getNextValidRank();
        }

        Pair<int, int> *initialTeamArray;
        initialTeamArray = new Pair<int, int>[initialNumTeams];

        currentTeam = firstTeamInRange;
        for (int i = 0; i < initialNumTeams; i++) {
            initialTeamArray[i].setKey(currentTeam->getTeamId());
            initialTeamArray[i].setValue(currentTeam->getTotalStats());
            currentTeam = currentTeam->getNextValidRank();
        }

        Pair<int, int> *prevArray = initialTeamArray;
        int currentTeamArraySize = initialNumTeams;
        Pair<int, int> *knockoutArray;
        while (currentTeamArraySize > 1) {

            if (currentTeamArraySize % 2 == 0) {
                currentTeamArraySize /= 2;

                knockoutArray = new Pair<int, int>[currentTeamArraySize];

                int arrayIndex = 0;
                for (int i = 1; i < initialNumTeams; i += 2) {
                    knockoutArray[arrayIndex] = compareKnockoutTeams(prevArray[i], prevArray[i - 1]);
                    arrayIndex++;
                }
                delete[] prevArray;
                prevArray = knockoutArray;
                initialNumTeams = currentTeamArraySize;
                continue;
            }

            if (currentTeamArraySize % 2 == 1) {
                currentTeamArraySize = ((currentTeamArraySize - 1) / 2) + 1;

                knockoutArray = new Pair<int, int>[currentTeamArraySize];

                int arrayIndex = 0;
                for (int i = 1; i < initialNumTeams; i += 2) {
                    knockoutArray[arrayIndex] = compareKnockoutTeams(prevArray[i], prevArray[i - 1]);
                    arrayIndex++;
                }
                knockoutArray[currentTeamArraySize - 1].setKey(prevArray[initialNumTeams - 1].getKey());
                knockoutArray[currentTeamArraySize - 1].setValue(prevArray[initialNumTeams - 1].getValue());
                delete[] prevArray;
                prevArray = knockoutArray;
                initialNumTeams = currentTeamArraySize;
            }
        }

        int winID = prevArray[0].getKey();
        delete[] prevArray;
        return winID;
    }

    catch(const KeyNotFound& keyNotFound) {
        return StatusType::FAILURE;
    }

    catch (const bad_alloc& badAlloc) {
        return StatusType::ALLOCATION_ERROR;
    }

}

Pair<int, int> world_cup_t::compareKnockoutTeams(const Pair<int, int> &firstTeam, const Pair<int, int> &secondTeam) {
    int newStats = firstTeam.getValue() + secondTeam.getValue() + 3;
    if(firstTeam.getValue() > secondTeam.getValue()) {
        Pair<int, int> newPair = Pair<int, int>(firstTeam.getKey(), newStats);
        return newPair;
    }

    if(firstTeam.getValue() < secondTeam.getValue()) {
        Pair<int, int> newPair = Pair<int, int>(secondTeam.getKey(), newStats);
        return newPair;
    }

    if(firstTeam.getValue() == secondTeam.getValue()) {
        if(firstTeam.getKey() > secondTeam.getKey()) {
            Pair<int, int> newPair = Pair<int, int>(firstTeam.getKey(), newStats);
            return newPair;
        }
        else {
            Pair<int, int> newPair = Pair<int, int>(secondTeam.getKey(), newStats);
            return newPair;
        }
    }
}
