#ifndef SHARED_ONLINE_LEADERBOARD_OPERATIONS
#define SHARED_ONLINE_LEADERBOARD_OPERATIONS

#include "shared/online/online_base.h"
#include "shared/online/leaderboard_data_types.h"

BEGIN_SHARED_NAMESPACE

BEGIN_ONLINE_NAMESPACE

class GetLeaderboardOperation final : public hermes::online::OnlineOperation
{
public:
	GetLeaderboardOperation(const std::string& servicePath);
	hermes::online::OnlineOperationType GetOnlineOperationType() const override;
};

class AddLeaderboardEntryOperation final : public hermes::online::OnlineOperation
{
public:
	AddLeaderboardEntryOperation(const std::string& servicePath, const LeaderboardEntry& entry);
	hermes::online::OnlineOperationType GetOnlineOperationType() const override;
};

END_ONLINE_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_ONLINE_LEADERBOARD_OPERATIONS