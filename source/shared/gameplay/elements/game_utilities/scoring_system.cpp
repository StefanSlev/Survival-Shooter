#include "shared/gameplay/elements/game_utilities/scoring_system.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

bool ScoringSystem::RegisterPlayer(int playerId)
{
	auto it = scores.find(playerId);

	if (it == scores.end())
	{
		scores[playerId] = 0;
		return true;
	}

	return false;
}

bool ScoringSystem::UnregisterPlayer(int playerId)
{
	auto it = scores.find(playerId);

	if (it != scores.end())
	{
		scores.erase(it);
		return true;
	}

	return false;
}

void ScoringSystem::AddScoreForPlayer(int playerID, int score)
{
	auto entry = scores.find(playerID);

	if (entry != scores.end())
		entry->second += score;
}

int ScoringSystem::GetPlayerScore(int playerID) const
{
	auto entry = scores.find(playerID);

	if (entry == scores.end())
		return N_A;

	return entry->second;
}

int ScoringSystem::GetGlobalScore() const
{
	int globalScore = 0;

	for (auto it = scores.begin(); it != scores.end(); ++it)
		globalScore += it->second;

	return globalScore;
}

int ScoringSystem::GetMaxScore() const
{
	int maxScore = 0;

	for (auto it = scores.begin(); it != scores.end(); ++it)
	{
		int currentScore = it->second;
		maxScore = (maxScore < currentScore) ? currentScore : maxScore;
	}

	return maxScore;
}

void ScoringSystem::Reset()
{
	scores.clear();
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE