#ifndef SHARED_GAMEPLAY_ELEMENTS_SCORING_SYSTEM
#define SHARED_GAMEPLAY_ELEMENTS_SCORING_SYSTEM

#include "shared/gameplay/gameplay_base.h"

// -------------------------------------------------------------------------------------
// Class that manages the score of the player
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class ScoringSystem : public hermes::system::Singleton<ScoringSystem>
{
	SINGLETON_ACCESS(ScoringSystem)

public:
	// not available
	const static int N_A = -1;

	// for one game session
	bool RegisterPlayer(int playerId);
	bool UnregisterPlayer(int playerId);
	void AddScoreForPlayer(int playerId, int score);

	int GetPlayerScore(int playerId) const;
	int GetGlobalScore() const;
	int GetMaxScore() const;
	
	void Reset();

private:
	std::unordered_map<int, int> scores;

	ScoringSystem() {}
	~ScoringSystem() {}
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#define GetScoringSystem() shared::gameplay::ScoringSystem::GetInstance()

#endif // SHARED_GAMEPLAY_ELEMENTS_SCORING_SYSTEM