#ifndef SHARED_ONLINE_LEADERBOARD_SERVICE
#define SHARED_ONLINE_LEADERBOARD_SERVICE

#include "shared/online/online_base.h"
#include "shared/online/leaderboard_data_types.h"

BEGIN_SHARED_NAMESPACE

BEGIN_ONLINE_NAMESPACE

class LeaderboardService final : public hermes::online::OnlineManager, public hermes::system::Singleton<LeaderboardService>
{
	SINGLETON_ACCESS(LeaderboardService)

public:
	void Initialize(const std::string& servicePath);
	
	void GetLeaderboard();
	void AddLeaderboardEntry(const LeaderboardEntry& entry);
	void OnOperationCompleted(hermes::online::OnlineOperationPtr onlineOperationPtr) override;

	const std::vector<LeaderboardEntry>& Leaderboard() const;

private:
	std::string m_servicePath;
	std::vector<LeaderboardEntry> m_leaderboard;
};

END_ONLINE_NAMESPACE

END_SHARED_NAMESPACE

#define GetLeaderboardService() shared::online::LeaderboardService::GetInstance()

#endif // SHARED_ONLINE_LEADERBOARD_SERVICE