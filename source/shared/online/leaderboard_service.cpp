#include "shared/online/leaderboard_service.h"
#include "shared/online/leaderboard_operations.h"

BEGIN_SHARED_NAMESPACE

BEGIN_ONLINE_NAMESPACE

void LeaderboardService::Initialize(const std::string& servicePath)
{
	this->m_servicePath = servicePath;
}

void LeaderboardService::GetLeaderboard()
{
	hermes::online::OnlineOperation* op = new GetLeaderboardOperation(m_servicePath);
	GetOnlineWorker().RegisterOnlineOperation(hermes::online::OnlineOperationPtr(op), this);
}

void LeaderboardService::AddLeaderboardEntry(const LeaderboardEntry& entry)
{
	hermes::online::OnlineOperation* op = new AddLeaderboardEntryOperation(m_servicePath, entry);
	GetOnlineWorker().RegisterOnlineOperation(hermes::online::OnlineOperationPtr(op), this);
}

void LeaderboardService::OnOperationCompleted(hermes::online::OnlineOperationPtr onlineOperationPtr)
{
	if (!onlineOperationPtr)
		return;

	switch (onlineOperationPtr->GetOnlineOperationType())
	{
	case hermes::online::OnlineOperationType::GetLeaderboard:
	{
		if (onlineOperationPtr->GetOperationStatus() == hermes::system::OperationStatus::Succeeded)
		{
			auto returnValuePtr = onlineOperationPtr->GetAsyncReturnValue().GetReturnValuePtr();
			auto leaderboardPtr = std::static_pointer_cast<shared::online::Leaderboard> (returnValuePtr);
			
			m_leaderboard = std::move(leaderboardPtr->entries);
		}

		break;
	}
	case hermes::online::OnlineOperationType::AddLeaderboardEntry:
		break;
	default:
		break;
	}
}

const std::vector<LeaderboardEntry>& LeaderboardService::Leaderboard() const
{
	return m_leaderboard;
}

END_ONLINE_NAMESPACE

END_SHARED_NAMESPACE